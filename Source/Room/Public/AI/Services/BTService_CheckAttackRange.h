// BTService_CheckAttackRange.h
// 플레이어와의 거리 및 감지 여부에 따라 AI의 상태(AIState)를 갱신하는 서비스
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckAttackRange.generated.h"

UCLASS()
class ROOM_API UBTService_CheckAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_CheckAttackRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
