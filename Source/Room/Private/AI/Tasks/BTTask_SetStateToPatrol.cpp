// BTTask_SetStateToPatrol.cpp
#include "AI/Tasks/BTTask_SetStateToPatrol.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Enums/AIEnums.h"

// 블랙보드 키: AIState
const FName UBTTask_SetStateToPatrol::BBKey_AIState(TEXT("AIState"));

UBTTask_SetStateToPatrol::UBTTask_SetStateToPatrol()
{
	NodeName = TEXT("Set AIState to Patrol");
}

EBTNodeResult::Type UBTTask_SetStateToPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	BlackboardComp->SetValueAsEnum(BBKey_AIState, (uint8)EAIStateType::Patrol);

	return EBTNodeResult::Succeeded;
}