// MeleeAttackComponent.h
// 근접 공격을 담당하는 AI 전용 컴포넌트
#pragma once

#include "CoreMinimal.h"
#include "BaseAttackComponent.h"
#include "MeleeAttackComponent.generated.h"

UCLASS()
class ROOM_API UMeleeAttackComponent : public UBaseAttackComponent
{
	GENERATED_BODY()
	
public:
	UMeleeAttackComponent();

	// AI의 전투 유형(Melee)을 반환
	virtual EAICombatType GetCombatType() const override { return EAICombatType::Melee; }

	// 애니메이션 실행 등 근접 공격 구현
	virtual void StartAttack() override;

	// 근접 공격 후 상태 초기화 등
	virtual void StopAttack() override;

	// 공격 애니메이션 길이 반환
	float GetAttackDuration() const;

	// 실제 공격 로직 수행 (노티파이에서 호출됨)
	void PerformMeleeAttack();

protected:
	// 공격 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage;
};

