// EnemyAIController.cpp
#include "AI/Controllers/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
//#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "AI/Interface/BaseAICharacter.h"
#include "GameFramework/Character.h"

// StaticDataSubsystem 관련 포함은 팀 프로젝트에서만 존재하므로 조건부 include
#if __has_include("Room/StaticData/StaticDataSubsystem.h")
#include "Room/StaticData/StaticDataSubsystem.h"
#include "Room/StaticData/StaticDataStruct.h"
#define USE_STATIC_DATA 1
#else
#define USE_STATIC_DATA 0
#endif

AEnemyAIController::AEnemyAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.f;
	SightConfig->LoseSightRadius = 1800.f;
	//SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->PeripheralVisionAngleDegrees = 110.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	// 타이머로 디버그 시각화
	GetWorldTimerManager().SetTimer(DebugDrawTimerHandle, this, &AEnemyAIController::DrawDebugSight, 1.0f, true);
#endif
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UE_LOG(LogTemp, Warning, TEXT("[AI] OnPossess called on %s"), *InPawn->GetName());

	ACharacter* EnemyCharacter = Cast<ACharacter>(InPawn);
	//if (!EnemyCharacter) return;
	if (!EnemyCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AI] OnPossess: Cast to ACharacter failed"));
		return;
	}

#if USE_STATIC_DATA
	// 팀 프로젝트: StaticDataSubsystem 활용
	UStaticDataSubsystem* SDS = GetGameInstance()->GetSubsystem<UStaticDataSubsystem>();
	if (!SDS) return;

	FName EnemyID = EnemyCharacter->Tags.IsValidIndex(0) ? EnemyCharacter->Tags[0] : FName("DefaultEnemy");
	const FEnemyData* EnemyData = SDS->GetEnemyData(EnemyID);
	if (!EnemyData) return;

	if (UseBlackboard(EnemyData->BehaviorTree->BlackboardAsset, BlackboardComp))
	{
		BlackboardComp->SetValueAsEnum(BBKey_AIState, static_cast<uint8>(EAIStateType::Idle));
		BlackboardComp->SetValueAsEnum(BBKey_CombatType, static_cast<uint8>(EnemyData->CombatType));
		RunBehaviorTree(EnemyData->BehaviorTree);
	}
#else
	if (BehaviorTreeAsset)
	{
		UBlackboardComponent* RawBlackboardComp = BlackboardComp.Get();
		if (UseBlackboard(BehaviorTreeAsset->BlackboardAsset, RawBlackboardComp))
		{
			BlackboardComp = RawBlackboardComp;
			UE_LOG(LogTemp, Warning, TEXT("[AI] UseBlackboard succeeded"));
			BlackboardComp->SetValueAsEnum(BBKey_AIState, static_cast<uint8>(EAIStateType::Idle));
			RunBehaviorTree(BehaviorTreeAsset);
			UE_LOG(LogTemp, Warning, TEXT("[AI] RunBehaviorTree called"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[AI] UseBlackboard failed"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AI] BehaviorTreeAsset is null"));
	}
#endif
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("[AI][%s] Perception update: Actor=%s, WasSensed=%d"), *GetName(), *Actor->GetName(), Stimulus.WasSuccessfullySensed());

	//if (!BlackboardComp) return;
	if (!BlackboardComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AI][%s] BlackboardComp is null!"), *GetName());
		return;
	}

	// 플레이어 Pawn 가져오기 (0번 인덱스 = 첫 번째 플레이어)
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// 인식된 Actor가 플레이어가 아니면 무시
	//if (Actor != PlayerPawn) return;
	if (Actor != PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AI][%s] Perceived actor is NOT player: %s"), *GetName(), *Actor->GetName());
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		BlackboardComp->SetValueAsObject(BBKey_TargetActor, Actor);
		BlackboardComp->SetValueAsVector(BBKey_TargetLastKnownLocation, Actor->GetActorLocation()); // 마지막 위치 기록
		BlackboardComp->SetValueAsEnum(BBKey_AIState, static_cast<uint8>(EAIStateType::Chase));

		UE_LOG(LogTemp, Log, TEXT("[AI][%s] Target detected: %s, switching AIState to Chase."), *GetName(), *Actor->GetName());

		// 타이머 시작해서 시야 시각화 실행
		if (!GetWorldTimerManager().IsTimerActive(DebugDrawTimerHandle))
		{
			GetWorldTimerManager().SetTimer(DebugDrawTimerHandle, this, &AEnemyAIController::DrawDebugSight, 0.1f, true);
		}
	}
	else
	{
		BlackboardComp->ClearValue(BBKey_TargetActor);
		BlackboardComp->SetValueAsVector(BBKey_TargetLastKnownLocation, Actor->GetActorLocation()); // 마지막 위치 기록
		BlackboardComp->SetValueAsEnum(BBKey_AIState, static_cast<uint8>(EAIStateType::Investigate));

		UE_LOG(LogTemp, Log, TEXT("[AI][%s] Lost target: %s, switching AIState to Investigate."), *GetName(), *Actor->GetName());

		// 타이머 멈춤 -> 시야 시각화 중지
		GetWorldTimerManager().ClearTimer(DebugDrawTimerHandle);
	}
}

void AEnemyAIController::DrawDebugSight()
{
	if (!GetPawn()) return;

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn || !SightConfig) return;

	UWorld* World = GetWorld();
	if (!World) return;



	FVector Origin = ControlledPawn->GetActorLocation() + FVector(0.f, 0.f, 50.f); // 눈 높이 보정
	FVector Forward = ControlledPawn->GetActorForwardVector();

	float SightRadius = SightConfig->SightRadius;
	float VisionAngle = SightConfig->PeripheralVisionAngleDegrees;

	int NumLines = 20; // 라인 개수 (조절 가능)
	float HalfAngleRad = FMath::DegreesToRadians(VisionAngle / 2.f);
	float AngleStep = (HalfAngleRad * 2) / NumLines;  // 전체 시야각을 NumLines로 나누기

	//UE_LOG(LogTemp, Warning, TEXT("SightRadius: %f"), SightRadius);
	for (int i = 0; i <= NumLines; i++)
	{
		// 왼쪽부터 오른쪽까지 각도 계산
		float CurrentAngle = -HalfAngleRad + AngleStep * i;

		// Forward 벡터 기준 Yaw 회전 벡터 얻기
		FRotator RotationOffset(0.f, FMath::RadiansToDegrees(CurrentAngle), 0.f);
		FVector Direction = RotationOffset.RotateVector(Forward);

		FVector TraceEnd = Origin + Direction * SightRadius;
		//UE_LOG(LogTemp, Warning, TEXT("TraceEnd: %s"), *TraceEnd.ToString());

		FHitResult HitResult;
		bool bHit = World->LineTraceSingleByChannel(HitResult, Origin, TraceEnd, ECC_Visibility);

		FVector LineEnd = bHit ? HitResult.Location : TraceEnd;

		// 디버그 라인 그리기
		DrawDebugLine(World, Origin, LineEnd, FColor::Green, false, 0.1f, 0, 1.5f);

	}
}

