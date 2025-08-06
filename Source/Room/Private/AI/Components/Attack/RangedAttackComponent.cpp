// RangedAttackComponent.cpp
#include "AI/Components/Attack/RangedAttackComponent.h"

URangedAttackComponent::URangedAttackComponent()
{
	CombatType = EAICombatType::Ranged;
}

void URangedAttackComponent::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("[AI][RangedAttack] 원거리 공격 수행!"));
	// TODO: 총알 발사 또는 투사체 스폰 처리
}
