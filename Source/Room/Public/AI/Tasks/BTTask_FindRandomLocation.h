// BTTask_FindRandomLocation.h
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindRandomLocation.generated.h"

UCLASS()
class ROOM_API UBTTask_FindRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindRandomLocation();

protected:
	// 실제 작업을 수행하는 ExecuteTask 함수
	// Behavior Tree가 이 Task를 실행할 때 호출되며, 작업의 성공/실패/진행 상태를 반환
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Blackboard에서 결과 위치를 저장할 키
	// 이 Task에서 찾은 랜덤 위치가 이 키에 저장
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector LocationKey;

	// 랜덤 위치를 찾기 위한 탐색 반경(Radius)
	UPROPERTY(EditAnywhere, Category = "Search", meta = (ClampMin = "100.0"))
	float SearchRadius = 1000.0f;

};

