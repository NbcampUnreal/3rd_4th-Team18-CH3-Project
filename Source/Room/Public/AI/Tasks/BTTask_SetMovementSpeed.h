// BTTask_SetMovementSpeed.h
// AI 캐릭터의 이동 속도를 설정하는 Task
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AI/Enums/AIEnums.h" // enum 포함 헤더
#include "BTTask_SetMovementSpeed.generated.h"

UCLASS()
class ROOM_API UBTTask_SetMovementSpeed : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetMovementSpeed();

protected:
	// Behavior Tree 디테일 패널에서 선택 가능한 속도 타입 (Walk, Run)
	UPROPERTY(EditAnywhere, Category = "Speed")
	ECharacterMovementSpeedType SpeedType;

	// ExecuteTask 함수 재정의
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

