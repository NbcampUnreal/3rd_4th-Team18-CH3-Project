// BTTask_PerformAttack.h
// AI가 실제 공격을 수행하도록 처리하는 Task
// 근거리/원거리 공격 컴포넌트를 확인해 StartAttack, StopAttack 호출 및 애니메이션 재생 등 처리
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PerformAttack.generated.h"

class UBaseAttackComponent;
class UMeleeAttackComponent;
class URangedAttackComponent;

UCLASS()
class ROOM_API UBTTask_PerformAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTTask_PerformAttack();

    // Task 실행
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    // Task 종료 시 호출 (정리용)
    virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

protected:
    // 공격 중 상태 플래그
    bool bIsAttacking = false;

    // 타이머 핸들
    //FTimerHandle TimerHandle_Attack;
    // Enemy별로 개별 타이머 핸들을 저장할 맵 추가
    UPROPERTY()
    TMap<TWeakObjectPtr<APawn>, FTimerHandle> PawnTimerHandles;

    // 공격 종료 후 호출되는 함수
    void OnAttackFinished(UBehaviorTreeComponent* OwnerComp);
};

