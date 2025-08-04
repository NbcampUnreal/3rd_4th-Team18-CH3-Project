// MeleeAttackComponent.cpp
#include "AI/Components/Attack/MeleeAttackComponent.h"

UMeleeAttackComponent::UMeleeAttackComponent()
{
	CombatType = EAICombatType::Melee;
}

void UMeleeAttackComponent::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("[AI][MeleeAttack] 근접 공격 수행!"));
	// TODO: 충돌 범위 내 액터에게 데미지 적용
}
