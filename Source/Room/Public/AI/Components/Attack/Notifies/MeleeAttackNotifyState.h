// MeleeAttackNotifyState.h
// 근접 공격 애니메이션 노티파이 상태 클래스
/*
	애니메이션 재생 중 특정 구간에서 근접 공격(피격 판정 등)을 실행하기 위해 사용.
	공격 애니메이션 시작 시점이나 실제 타격 프레임에 이 노티파이를 붙임.
*/
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MeleeAttackNotifyState.generated.h"

UCLASS()
class ROOM_API UMeleeAttackNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	// 애니메이션 노티파이 구간이 시작될 때 호출되는 함수
	/*
		이 함수 안에서 근접 공격 컴포넌트의 공격 판정 함수를 호출하는 로직을 작성
		@param MeshComp 공격 애니메이션이 재생 중인 SkeletalMesh 컴포넌트
		@param Animation 현재 재생 중인 애니메이션 시퀀스
		@param TotalDuration 노티파이 구간의 총 길이(초)
	*/
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
};
