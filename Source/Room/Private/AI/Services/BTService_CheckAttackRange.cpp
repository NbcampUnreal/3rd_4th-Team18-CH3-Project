// BTService_CheckAttackRange.cpp
#include "AI/Services/BTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "AI/Components/Attack/BaseAttackComponent.h"
#include "Components/CapsuleComponent.h"
#include "AI/Enums/AIEnums.h"
#include "NavigationSystem.h"

UBTService_CheckAttackRange::UBTService_CheckAttackRange()
{
	// 서비스 이름 설정 (에디터에서 트리 노드 이름으로 표시됨)
	NodeName = TEXT("Check Attack Range");

	// 서비스 실행 간격 설정
	Interval = 0.2f; // 0.2초마다 TickNode()를 호출

	// 실행 주기에 무작위 오차를 추가 (성능 분산 목적)
	// 여러 AI가 동시에 Tick을 실행하지 않도록 약간의 무작위성 부여
	RandomDeviation = 0.1f;

	// 이 노드가 활성화되었을 때 알림을 받을지 여부 (시작 시점에 알림 수신)
	// true 설정 시, 서비스가 활성화될 때 OnBecomeRelevant() 호출됨
	// OnBecomeRelevant : 서비스가 처음 시작될 때 초기화 작업을 하거나, 한 번만 실행해도 되는 작업을 처리할 때 사용
	bNotifyBecomeRelevant = true;

	// 이 노드가 더 이상 활성 상태가 아닐 때 알림을 받을지 여부
	// false면 OnCeaseRelevant()는 호출되지 않음
	// OnCeaseRelevant : Behavior Tree 서비스가 더 이상 활성화되지 않을 때 호출되는 함수
	// 보통 서비스가 끝날 때 정리 작업(cleanup), 상태 초기화, 타이머 해제, 리소스 해제 등을 처리할 때 사용
	bNotifyCeaseRelevant = false;
}

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return;

	// Blackboard 키: TargetActor와 IsPlayerVisible 가져오기
	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TEXT("TargetActor")));
	bool bIsPlayerVisible = Blackboard->GetValueAsBool(TEXT("IsPlayerVisible"));

	// 감지 중이 아니거나 타겟이 없으면 Investigate or Patrol 상태로 전환
	if (!bIsPlayerVisible || !TargetActor)
	{
		const FVector LastKnownLocation = Blackboard->GetValueAsVector(TEXT("TargetLastKnownLocation"));

		// 벡터가 세팅 안 돼 있으면 Patrol
		if (!Blackboard->IsVectorValueSet(TEXT("TargetLastKnownLocation")))
		{
			Blackboard->SetValueAsEnum(TEXT("AIState"), static_cast<uint8>(EAIStateType::Patrol));
		}
		else
		{// 위치는 세팅돼 있지만 NavMesh 위인지 확인
			UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
			FNavLocation ProjectedLocation;

			if (!NavSys || !NavSys->ProjectPointToNavigation(LastKnownLocation, ProjectedLocation))
			{// NavMesh 위가 아니면 Patrol
				Blackboard->SetValueAsEnum(TEXT("AIState"), static_cast<uint8>(EAIStateType::Patrol));
			}
			else
			{// 유효한 위치면 Investigate
				Blackboard->SetValueAsEnum(TEXT("AIState"), static_cast<uint8>(EAIStateType::Investigate));
			}
		}
		return;
	}

	// 공격 컴포넌트 가져오기
	UBaseAttackComponent* AttackComp = AIPawn->FindComponentByClass<UBaseAttackComponent>();
	if (!AttackComp)
	{// 없으면 기본적으로 Chase 상태 유지
		Blackboard->SetValueAsEnum(TEXT("AIState"), static_cast<uint8>(EAIStateType::Chase));
		return;
	}

	// 공격 사거리
	const float AttackRange = AttackComp->GetAttackData().AttackRange;

	// 캡슐 컴포넌트 가져오기
	UCapsuleComponent* MyCapsule = AIPawn->FindComponentByClass<UCapsuleComponent>();
	UCapsuleComponent* TargetCapsule = TargetActor->FindComponentByClass<UCapsuleComponent>();

	// 캡슐 위치 (없으면 Actor Location)
	FVector MyLocation = MyCapsule ? MyCapsule->GetComponentLocation() : AIPawn->GetActorLocation();
	FVector TargetLocation = TargetCapsule ? TargetCapsule->GetComponentLocation() : TargetActor->GetActorLocation();

	// 캡슐 반지름
	const float MyRadius = MyCapsule ? MyCapsule->GetScaledCapsuleRadius() : 0.f;
	const float TargetRadius = TargetCapsule ? TargetCapsule->GetScaledCapsuleRadius() : 0.f;

	// 원본 거리
	const float RawDistance = FVector::Dist(MyLocation, TargetLocation);

	// 보정된 거리 (Capsule 반지름 빼줌)
	const float AdjustedDistance = RawDistance - (MyRadius + TargetRadius);

	// 범위 내에 있으면 Attack, 아니면 Chase
	const bool bInRange = AdjustedDistance <= AttackRange;
	EAIStateType NewState = bInRange ? EAIStateType::Attack : EAIStateType::Chase;

	// Blackboard에 상태 저장
	Blackboard->SetValueAsEnum(TEXT("AIState"), static_cast<uint8>(NewState));

#if WITH_EDITOR
	// 디버그 출력
	//UE_LOG(LogTemp, Log, TEXT("[AI][BTService] TargetActor: %s | IsPlayerVisible: %s"),
	//	TargetActor ? *TargetActor->GetName() : TEXT("NULL"),
	//	bIsPlayerVisible ? TEXT("true") : TEXT("false"));

	//UE_LOG(LogTemp, Log, TEXT("[AI][BTService] AttackComp: %s | MyCapsule: %s | TargetCapsule: %s"),
	//	AttackComp ? TEXT("Valid") : TEXT("NULL"),
	//	MyCapsule ? TEXT("Valid") : TEXT("NULL"),
	//	TargetCapsule ? TEXT("Valid") : TEXT("NULL"));

	//UE_LOG(LogTemp, Log, TEXT("[AI][BTService] Distance: %.2f | Adjusted: %.2f | Range: %.2f → State: %s"),
	//	RawDistance, AdjustedDistance, AttackRange,
	//	*UEnum::GetValueAsString(NewState));

	//UE_LOG(LogTemp, Log, TEXT("[AI][BTService] Final State Set: %s"),
	//	*UEnum::GetValueAsString(NewState));
#endif
}
