// BTTask_ClearFocus.h
// AI의 Focus(시선 고정)를 해제하는 Task
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ClearFocus.generated.h"

UCLASS()
class ROOM_API UBTTask_ClearFocus : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ClearFocus();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

