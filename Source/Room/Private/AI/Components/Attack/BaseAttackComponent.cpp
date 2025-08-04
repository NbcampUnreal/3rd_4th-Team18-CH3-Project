// BaseAttackComponent.cpp
#include "AI/Components/Attack/BaseAttackComponent.h"

UBaseAttackComponent::UBaseAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CombatType = EAICombatType::Melee;
}

void UBaseAttackComponent::Attack()
{
	// 기본 공격 로직 없음
	UE_LOG(LogTemp, Warning, TEXT("[AI][BaseAttackComponent] 기본 공격 실행"));
}