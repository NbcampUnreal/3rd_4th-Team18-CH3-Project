// EnemyAIController.cpp
#include "AI/Controllers/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
//#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "AI/Interface/BaseAICharacterInterface.h"
#include "GameFramework/Character.h"
#include "AI/Components/Attack/BaseAttackComponent.h"
#include "Components/CapsuleComponent.h"
#include "NavigationSystem.h"
#include "Define/GameDefine.h"


// StaticDataSubsystem 관련 포함은 팀 프로젝트에서만 존재하므로 조건부 include
//#if __has_include("Room/StaticData/StaticDataSubsystem.h")
//#include "Room/StaticData/StaticDataSubsystem.h"
//#include "Room/StaticData/StaticDataStruct.h"
//#define USE_STATIC_DATA 1
//#else
//#define USE_STATIC_DATA 0
//#endif

// 블랙보드 키 초기화 (정적 상수 정의)
const FName AEnemyAIController::BBKey_TargetActor(TEXT("TargetActor"));
const FName AEnemyAIController::BBKey_TargetLastKnownLocation(TEXT("TargetLastKnownLocation"));
const FName AEnemyAIController::BBKey_AIState(TEXT("AIState"));
const FName AEnemyAIController::BBKey_CombatType(TEXT("CombatType"));
const FName AEnemyAIController::BBKey_IsPlayerVisible = TEXT("IsPlayerVisible");

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	// 블랙보드 컴포넌트를 생성
	// -> AI가 기억할 정보를 저장하고 비헤이비어 트리에서 사용하는 변수들을 관리
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	// AI 감지 시스템 (Perception) 컴포넌트 생성 및 설정
	// -> 시야, 청각 등 다양한 센스를 통해 타겟을 감지할 수 있게 해주는 핵심 컴포넌트
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	// 이 AIController가 사용할 인식 컴포넌트를 지정
	// -> PerceptionComponent 프로퍼티에 AIPerception을 할당
	SetPerceptionComponent(*AIPerception);

	//=========================================
	// 시야 감지 구성 설정
	//=========================================
	// 시야 감지 설정 컴포넌트를 생성
	// -> 시야 범위, 각도, 감지 지속 시간 등을 설정할 수 있는 구성 요소
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// 탐지 범위의 반경. 이 거리 이내에 대상이 있으면 인식 시작
	SightConfig->SightRadius = 1500.f;

	// 인식된 대상이 이 거리 바깥으로 나가야만 인식 해제
	SightConfig->LoseSightRadius = 1800.f;

	// 좌우 시야각. 정면 기준 ±55도까지 감지. 즉, 총 110도 시야
	//SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->PeripheralVisionAngleDegrees = 110.f;

	// 대상이 감지에서 사라진 후, 5초 동안 기억됩니다. 이후 잊음.
	SightConfig->SetMaxAge(5.f);

	// 감지 가능 액터 설정
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;		// 적대적인 액터
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;	// 우호적인 액터도 감지 가능
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;		// 중립적인 액터도 감지 가능

	//=========================================
	// 감지 구성 등록 및 우선 감지 방식 지정
	//=========================================
	// 어떤 감지를 사용할지 설정
	// AIPerception에 시야 감지(Sight) 설정을 등록 -> 이 설정(SightConfig)은 시야 범위, 각도, 감지 지속 시간 등을 포함함
	AIPerception->ConfigureSense(*SightConfig);

	// 우선 감지 방식을 지정
	// AI가 사용하는 기본 감지 센스를 시야(Sight)로 설정 -> 여러 감지 센스가 있을 경우, 가장 우선시되는 감지를 지정함
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	// 감지가 발생했을 때 실행될 함수를 바인딩
	// 감지 이벤트가 발생했을 때 호출할 델리게이트 함수(OnTargetPerceptionUpdated)를 등록 -> 대상 감지 시 or 시야에서 사라질 때 자동 호출됨
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAIController::Tick(float DeltaTime)
{// 타겟이 존재할 경우, 디버그용 시각화 (라인 및 스피어)
	Super::Tick(DeltaTime);

	if (!bDrawDebug) return;
	if (!BlackboardComp) return;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(BBKey_TargetActor));
	if (!TargetActor) return;

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	UCapsuleComponent* MyCapsule = ControlledPawn->FindComponentByClass<UCapsuleComponent>();
	UCapsuleComponent* TargetCapsule = TargetActor->FindComponentByClass<UCapsuleComponent>();

	FVector MyLocation = MyCapsule ? MyCapsule->GetComponentLocation() : ControlledPawn->GetActorLocation();
	FVector TargetLocation = TargetCapsule ? TargetCapsule->GetComponentLocation() : TargetActor->GetActorLocation();

	//MyLocation.Z = 0.f;
	//TargetLocation.Z = 0.f;

	DrawDebugLine(GetWorld(), MyLocation, TargetLocation, FColor::Red, false, -1.f, 0, 2.0f);
	DrawDebugSphere(GetWorld(), MyLocation, 30.f, 12, FColor::Green, false, -1.f);
	DrawDebugSphere(GetWorld(), TargetLocation, 30.f, 12, FColor::Blue, false, -1.f);

	// 공격 사거리 표시 (빨간색 스피어)
	UBaseAttackComponent* AttackComp = ControlledPawn->FindComponentByClass<UBaseAttackComponent>();
	if (AttackComp)
	{
		const float AttackRange = AttackComp->GetAttackData().AttackRange;
		const float MyRadius = MyCapsule ? MyCapsule->GetScaledCapsuleRadius() : 0.f;
		const float TargetRadius = TargetCapsule ? TargetCapsule->GetScaledCapsuleRadius() : 0.f;

		DrawDebugSphere(GetWorld(), MyLocation, AttackRange + MyRadius + TargetRadius, 32, FColor::Red, false, -1.f);
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UE_LOG(LogTemp, Warning, TEXT("[AI][EnemyAIController] OnPossess called on %s"), *InPawn->GetName());

	ACharacter* EnemyCharacter = Cast<ACharacter>(InPawn);
	if (!EnemyCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AI][EnemyAIController] OnPossess: Cast to ACharacter failed"));
		return;
	}

//#if USE_STATIC_DATA
//	// 팀 프로젝트: StaticDataSubsystem 활용
//	UStaticDataSubsystem* SDS = GetGameInstance()->GetSubsystem<UStaticDataSubsystem>();
//	if (!SDS) return;
//
//	FName EnemyID = EnemyCharacter->Tags.IsValidIndex(0) ? EnemyCharacter->Tags[0] : FName("DefaultEnemy");
//	const FEnemyData* EnemyData = SDS->GetEnemyData(EnemyID);
//	if (!EnemyData) return;
//
//	if (UseBlackboard(EnemyData->BehaviorTree->BlackboardAsset, BlackboardComp))
//	{
//		BlackboardComp->SetValueAsEnum(BBKey_AIState, static_cast<uint8>(EAIStateType::Idle));
//		BlackboardComp->SetValueAsEnum(BBKey_CombatType, static_cast<uint8>(EnemyData->CombatType));
//		RunBehaviorTree(EnemyData->BehaviorTree);
//	}
//#else
	if (BehaviorTreeAsset)
	{
		UBlackboardComponent* RawBlackboardComp = BlackboardComp.Get();
		if (UseBlackboard(BehaviorTreeAsset->BlackboardAsset, RawBlackboardComp))
		{
			BlackboardComp = RawBlackboardComp;
			//UE_LOG(LogTemp, Warning, TEXT("[AI][EnemyAIController] UseBlackboard succeeded"));

			// 기본 AI 상태 설정
			BlackboardComp->SetValueAsEnum(BBKey_AIState, static_cast<uint8>(EAIStateType::Idle));

			// CombatType을 AttackComponent에서 가져와서 BB에 설정
			SetCombatTypeToBlackboard(EnemyCharacter);

			RunBehaviorTree(BehaviorTreeAsset);
			//UE_LOG(LogTemp, Warning, TEXT("[AI][EnemyAIController] RunBehaviorTree called"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[AI][EnemyAIController] UseBlackboard failed"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AI][EnemyAIController] BehaviorTreeAsset is null"));
	}
//#endif
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// 블랙보드가 없으면 처리 중단
	if (!BlackboardComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AI][EnemyAIController][%s] BlackboardComp is null!"), *GetName());
		return;
	}

	// 플레이어 Pawn 가져오기 (플레이어 0번 인덱스 기준)
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	// 감지된 Actor가 플레이어가 아니면 무시
	if (Actor->ActorHasTag(GameDefine::PlayerTag.GetTagName()))
	{
		return;
	}

	// 감지 성공 여부
	// true: 감지 성공 (시야 안에 들어옴), false: 감지 실패 (시야에서 사라짐)
	const bool bSensed = Stimulus.WasSuccessfullySensed();

	// 블랙보드에 플레이어 감지 여부 업데이트
	BlackboardComp->SetValueAsBool(BBKey_IsPlayerVisible, bSensed);

	// AI 자신의 위치와 감지 대상의 위치를 가져옴
	const FVector MyLocation = GetPawn() ? GetPawn()->GetActorLocation() : FVector::ZeroVector;
	const FVector TargetLocation = Actor->GetActorLocation();

	// 네비게이션 시스템 참조 -> 위치가 내비게이션 가능 영역인지 확인하는 데 사용
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation ProjectedLocation; // 내비게이션 영역으로 투영된 위치 저장용

	// 위치 업데이트 최소 거리 제약 조건 (너무 가까우면 블랙보드 갱신 생략)
	const float MinUpdateDistanceSq = FMath::Square(100.0f);

	//================================================ 감지 성공 시 로직 ================================================ 
	if (bSensed)
	{
		// 블랙보드에 타겟 액터 설정
		BlackboardComp->SetValueAsObject(BBKey_TargetActor, Actor);

		// 일정 거리 이상 떨어져 있을 때만 LastKnownLocation 갱신
		if (FVector::DistSquared(TargetLocation, MyLocation) > MinUpdateDistanceSq)
		{
			// 감지 위치를 내비게이션 맵 상으로 투영 (유효한 이동 지점인지 확인)
			if (NavSys && NavSys->ProjectPointToNavigation(TargetLocation, ProjectedLocation))
			{
				// 타겟의 마지막 위치를 블랙보드에 기록
				BlackboardComp->SetValueAsVector(BBKey_TargetLastKnownLocation, ProjectedLocation.Location);
				//UE_LOG(LogTemp, Warning, TEXT("[AI][%s] Target sensed: Updating TargetActor & Location: %s"), *GetName(), *ProjectedLocation.Location.ToString());
			}
			else
			{// 투영 실패 시 (ex. 내비 영역 밖) 업데이트 생략
				UE_LOG(LogTemp, Warning, TEXT("[AI][EnemyAIController][%s] Target sensed: Location not on navmesh, skipping update."), *GetName());
			}
		}
		else
		{// 거리가 너무 가까워 업데이트 생략
			//UE_LOG(LogTemp, Warning, TEXT("[AI][%s] Target sensed: Too close to update LastKnownLocation."), *GetName());
		}
	}
	//================================================ 감지 실패 시 로직 ================================================ 
	else
	{// 감지 실패 시: TargetActor 클리어 + 마지막 위치만 저장 (단, 유효할 때만)

		// 타겟 Actor 블랙보드에서 제거
		BlackboardComp->ClearValue(BBKey_TargetActor);

		// 여전히 일정 거리 이상 떨어져 있을 경우 마지막 위치 업데이트
		if (FVector::DistSquared(TargetLocation, MyLocation) > MinUpdateDistanceSq)
		{
			if (NavSys && NavSys->ProjectPointToNavigation(TargetLocation, ProjectedLocation))
			{
				// 감지 실패 시에도 타겟의 마지막 위치는 저장해 추적 행동 등에 활용 가능
				BlackboardComp->SetValueAsVector(BBKey_TargetLastKnownLocation, ProjectedLocation.Location);
				//UE_LOG(LogTemp, Warning, TEXT("[AI][%s] Target lost: Updating LastKnownLocation: %s"), *GetName(), *ProjectedLocation.Location.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[AI][EnemyAIController][%s] Target lost: Location not on navmesh, skipping update."), *GetName());
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("[AI][%s] Target lost: Too close to update LastKnownLocation."), *GetName());
		}
	}
}

void AEnemyAIController::SetCombatTypeToBlackboard(ACharacter* EnemyCharacter)
{
	// 전달된 EnemyCharacter가 유효하지 않거나, Blackboard가 존재하지 않으면 함수 종료
	if (!EnemyCharacter || !BlackboardComp) return;

	// EnemyCharacter에서 BaseAttackComponent를 찾아서 가져옴
	// AttackComponent에서 CombatType 가져오기
	UBaseAttackComponent* AttackComponent = EnemyCharacter->FindComponentByClass<UBaseAttackComponent>();
	if (AttackComponent)
	{
		// 컴포넌트로부터 전투 타입 정보를 가져옴 (None, Melee, Ranged, Mixed)
		EAICombatType CombatType = AttackComponent->GetCombatType();

		// 블랙보드에 해당 전투 타입을 Enum 형태로 저장
		BlackboardComp->SetValueAsEnum(TEXT("AICombatType"), static_cast<uint8>(CombatType));

		// 디버그 로그
		//FString CombatTypeString;
		//switch (CombatType)
		//{
		//case EAICombatType::None: CombatTypeString = TEXT("None"); break;
		//case EAICombatType::Melee: CombatTypeString = TEXT("Melee"); break;
		//case EAICombatType::Ranged: CombatTypeString = TEXT("Ranged"); break;
		//case EAICombatType::Mixed: CombatTypeString = TEXT("Mixed"); break;
		//}
		//UE_LOG(LogTemp, Warning, TEXT("[AI] CombatType set to BB: %s for %s"),
		//	*CombatTypeString, *EnemyCharacter->GetName());
	}
	else
	{// 공격 컴포넌트를 찾지 못한 경우 경고 로그 출력
		UE_LOG(LogTemp, Warning, TEXT("[AI][EnemyAIController] No AttackComponent found on %s"), *EnemyCharacter->GetName());
	}
}
