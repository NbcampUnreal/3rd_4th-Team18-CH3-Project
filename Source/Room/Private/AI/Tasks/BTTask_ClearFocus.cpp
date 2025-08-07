// BTTask_ClearFocus.cpp
#include "AI/Tasks/BTTask_ClearFocus.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearFocus::UBTTask_ClearFocus()
{
    NodeName = TEXT("Clear Focus");
}

EBTNodeResult::Type UBTTask_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        UE_LOG(LogTemp, Warning, TEXT("[AI][ClearFocus] No valid AIController"));
        return EBTNodeResult::Failed;
    }

    // Gameplay 우선순위 시선 고정 해제
    AIController->ClearFocus(EAIFocusPriority::Gameplay);

    //UE_LOG(LogTemp, Log, TEXT("[AI][ClearFocus] Focus cleared"));
    return EBTNodeResult::Succeeded;
}
