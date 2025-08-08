// RangedAttackNotifyState.h
// 원거리 공격 애니메이션 노티파이 상태 클래스
/*
	애니메이션 재생 중 특정 구간에서 원거리 공격(총알 발사 등)을 실행하기 위해 사용
*/
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RangedAttackNotifyState.generated.h"

UCLASS()
class ROOM_API URangedAttackNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	// 애니메이션 노티파이 시작 시 호출
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	// 필요하면 NotifyTick, NotifyEnd도 오버라이드 가능
};
