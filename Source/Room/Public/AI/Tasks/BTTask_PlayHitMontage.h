// BTTask_PlayHitMontage.h
// AI가 피격 상태일 때 피격 애니메이션을 재생하는 Task
/*
	캐릭터의 RunMontage(ECharacterAnim::Hited)를 호출하여 몽타주 재생.
	애니메이션 재생 자체는 캐릭터 내부에서 처리됨
*/
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PlayHitMontage.generated.h"

UCLASS()
class ROOM_API UBTTask_PlayHitMontage : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_PlayHitMontage();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
