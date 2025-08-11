// BTDecorator_Timeout.cpp
#include "AI/Decorators/BTDecorator_Timeout.h"
#include "Engine/World.h"

UBTDecorator_Timeout::UBTDecorator_Timeout()
{
    NodeName = TEXT("Timeout"); // 에디터에서 노드 이름으로 표시됨
    bNotifyTick = true;         // 매 Tick 마다 TickNode 함수 호출 허용
}

void UBTDecorator_Timeout::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
    FTimeoutMemory* TimeoutMemory = reinterpret_cast<FTimeoutMemory*>(NodeMemory);
    TimeoutMemory->ElapsedTime = 0.f; // 초기화 시 시간 0으로 설정
}

void UBTDecorator_Timeout::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    FTimeoutMemory* TimeoutMemory = reinterpret_cast<FTimeoutMemory*>(NodeMemory);
    TimeoutMemory->ElapsedTime += DeltaSeconds; // 프레임마다 시간 누적

    if (TimeoutMemory->ElapsedTime >= TimeoutDuration)
    {
        // 타임아웃이 지나면 강제로 조건 실패 상태를 만들기 위해 RequestExecution 요청
        // 조건 실패로 판단되도록 유도
        OwnerComp.RequestExecution(this);

        //UE_LOG(LogTemp, Warning, TEXT("[AI][Timeout] Timeout reached, failing decorator"));
    }
}

bool UBTDecorator_Timeout::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const FTimeoutMemory* TimeoutMemory = reinterpret_cast<const FTimeoutMemory*>(NodeMemory);
    return TimeoutMemory->ElapsedTime < TimeoutDuration;  // 시간 초과 시 false 리턴 -> 노드 실패
}