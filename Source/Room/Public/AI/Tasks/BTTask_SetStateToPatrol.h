// BTTask_SetStateToPatrol.h
// 블랙보드에 지정된 AIState 값을 "Patrol"로 설정하는 Task
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetStateToPatrol.generated.h"

UCLASS()
class ROOM_API UBTTask_SetStateToPatrol : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SetStateToPatrol();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// AIState를 설정할 블랙보드 키 이름
	static const FName BBKey_AIState;
};

