// BTTask_MaintainOptimalDistance.h
// AI가 타겟과의 거리를 유지하기 위해, 너무 가까우면 도망가고 너무 멀면 접근하는 로직을 수행하는 Task
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MaintainOptimalDistance.generated.h"

UCLASS()
class ROOM_API UBTTask_MaintainOptimalDistance : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTTask_MaintainOptimalDistance();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    // AttackRange 기준 거리 비율 설정
    UPROPERTY(EditAnywhere, Category = "Distance Ratios", meta = (ClampMin = "0.5", ClampMax = "1.0"))
    float TooCloseRatio = 0.7f; // AttackRange의 70%보다 가까우면 후진

    UPROPERTY(EditAnywhere, Category = "Distance Ratios", meta = (ClampMin = "1.0", ClampMax = "2.0"))
    float TooFarRatio = 1.3f;   // AttackRange의 130%보다 멀면 접근

    // 이동 속도
    UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "100.0", ClampMax = "1000.0"))
    float MovementSpeed = 200.0f;
};
