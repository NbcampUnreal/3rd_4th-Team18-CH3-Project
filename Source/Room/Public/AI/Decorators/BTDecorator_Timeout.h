// BTDecorator_Timeout.h
// 일정 시간이 경과할 때까지 false를 반환하다가 설정된 TimeoutDuration이 지나면 true를 반환하는 BT 데코레이터
/*
    주 용도:
    - 특정 행동을 일정 시간 동안 막거나 지연시킴
    - 쿨타임 같은 타이머 로직을 Behavior Tree에서 구현 가능
*/
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_Timeout.generated.h"

UCLASS()
class ROOM_API UBTDecorator_Timeout : public UBTDecorator
{
	GENERATED_BODY()
	
public:
    UBTDecorator_Timeout();

protected:
    // 타임아웃 시간 (초)
    UPROPERTY(EditAnywhere, Category = "Timeout")
    float TimeoutDuration = 5.0f;

    // 데코레이터의 인스턴스 메모리
    /*
        각 BT 인스턴스마다 별도로 유지됨. 이 안에 경과 시간 저장
    */
    struct FTimeoutMemory
    {
        float ElapsedTime = 0.f;
    };

    // 데코레이터가 사용할 메모리의 크기를 리턴 - BT 실행 중 상태 유지를 위해 필요한 구조체 크기를 반환
    virtual uint16 GetInstanceMemorySize() const override { return sizeof(FTimeoutMemory); }

    // 메모리 초기화 시 호출 - ElapsedTime 초기화
    virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;

    // 매 프레임마다 호출되어 시간 누적 - TimeoutDuration에 도달할 때까지 ElapsedTime 증가
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    // 데코레이터 조건 계산 함수 - ElapsedTime이 TimeoutDuration보다 크거나 같으면 true 반환 그렇지 않으면 false
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

