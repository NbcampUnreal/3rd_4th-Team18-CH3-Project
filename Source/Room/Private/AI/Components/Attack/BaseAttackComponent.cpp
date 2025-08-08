// BaseAttackComponent.cpp
#include "AI/Components/Attack/BaseAttackComponent.h"

UBaseAttackComponent::UBaseAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UBaseAttackComponent::CanAttack() const
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	// 공격 중이 아니고, 마지막 공격 이후 쿨다운이 지났으면 공격 가능
	bool bCanAttack = !bIsAttacking && (CurrentTime - LastAttackTime) >= AttackData.AttackCooldown;

	// DEBUG : 공격 가능 여부 출력
	//UE_LOG(LogTemp, Log, TEXT("[AI][BaseAttackComponent][%s][CanAttack] bIsAttacking=%d, LastAttackTime=%.2f, CurrentTime=%.2f, CanAttack=%d"),
	//	*GetOwner()->GetName(), bIsAttacking, LastAttackTime, CurrentTime, bCanAttack);

	return bCanAttack;
}

void UBaseAttackComponent::StartAttack()
{
	if (!CanAttack())
	{// 공격할 수 없는 상황이면 경고 출력 후 리턴
		UE_LOG(LogTemp, Warning, TEXT("[AI][BaseAttackComponent][%s][StartAttack] Cannot start attack (CanAttack returned false)"), *GetOwner()->GetName());
		return;
	}

	// 공격 상태로 전환
	bIsAttacking = true;

	// 현재 시간을 마지막 공격 시간으로 갱신
	LastAttackTime = GetWorld()->GetTimeSeconds();

	// DEBUG : 공격 시작 시간 출력
	//UE_LOG(LogTemp, Log, TEXT("[AI][BaseAttackComponent][%s][StartAttack] Attack started at %.2f"), *GetOwner()->GetName(), LastAttackTime);
}

void UBaseAttackComponent::StopAttack()
{
	// 공격 상태 종료
	bIsAttacking = false;

	// DEBUG : 공격 중단 알림
	//UE_LOG(LogTemp, Log, TEXT("[AI][BaseAttackComponent][%s][StopAttack] Attack stopped"), *GetOwner()->GetName());
}