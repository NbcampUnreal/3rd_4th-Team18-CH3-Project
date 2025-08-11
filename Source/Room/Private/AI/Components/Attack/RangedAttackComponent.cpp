// RangedAttackComponent.cpp
#include "AI/Components/Attack/RangedAttackComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

URangedAttackComponent::URangedAttackComponent()
{
	// 기본 원거리 공격 설정값 초기화
	AttackData.AttackRange = 800.f;		// 사거리: 8m
	AttackData.AttackDamage = 10.f;		// 데미지: 10
	AttackData.AttackCooldown = 2.5f;	// 쿨타임: 2.5초
	// AttackRadius는 근접 공격만 사용하므로 무시
}

/*
	StartAttack() 호출 시의 전체 흐름

	1. 공격 가능 여부 확인 (쿨타임, 현재 공격 중 여부 등)
	2. 공격 상태 활성화 (bIsAttacking = true) + 마지막 공격 시간 저장
	3. 애니메이션 몽타주 재생
	4. AnimNotify에서 발사체 생성 또는 데미지 적용
	5. 애니메이션 종료 후 StopAttack() 호출 -> 상태 정리
*/
void URangedAttackComponent::StartAttack()
{
	// 공격이 가능한 상태인지 확인
	if (!CanAttack()) return;

	// 공격 상태 활성화 + 마지막 공격 시간 기록
	bIsAttacking = true;
	LastAttackTime = GetWorld()->GetTimeSeconds();

	// 애니메이션 몽타주 실행
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && AttackMontage)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(AttackMontage);
			//UE_LOG(LogTemp, Log, TEXT("[AI][RangedAttack] Montage Played for %s"), *GetOwner()->GetName());
		}
	}
	else
	{// Owner 또는 AttackMontage가 유효하지 않은 경우 경고 로그
		UE_LOG(LogTemp, Warning, TEXT("[AI][RangedAttack] Cannot play montage: Owner or Montage invalid"));
	}
}

void URangedAttackComponent::StopAttack()
{
	// 공격 상태 비활성화
	bIsAttacking = false;

	// 현재 재생 중인 몽타주가 있다면 정지
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && AttackMontage)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && AnimInstance->Montage_IsPlaying(AttackMontage))
		{
			// 부드럽게 정지 (페이드 아웃 포함)
			AnimInstance->Montage_Stop(0.25f, AttackMontage);
			//UE_LOG(LogTemp, Log, TEXT("[AI][RangedAttack] Montage stopped for %s"), *GetOwner()->GetName());
		}
	}

	//UE_LOG(LogTemp, Log, TEXT("[AI][RangedAttack] StopAttack called for %s"), *GetOwner()->GetName());
}

float URangedAttackComponent::GetAttackDuration() const
{
	// 애니메이션 몽타주가 유효할 경우 길이 반환, 없으면 기본값 사용
	return AttackMontage ? AttackMontage->GetPlayLength() : AttackData.AttackCooldown;
}

void URangedAttackComponent::PerformRangedAttack()
{
	if (!GetOwner())
		return;

	// TODO: 총알 클래스가 준비되면 아래 부분을 구현할 것
	/*
	if (!ProjectileClass)
		return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
		return;

	FVector MuzzleLocation = GetComponentLocation(); // 또는 총구 소켓 위치
	FRotator MuzzleRotation = GetComponentRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = OwnerCharacter;
	SpawnParams.Owner = OwnerCharacter;

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(
		ProjectileClass,
		MuzzleLocation,
		MuzzleRotation,
		SpawnParams
	);

	if (Projectile)
	{
		Projectile->FireInDirection(MuzzleRotation.Vector());
	}
	*/

	UE_LOG(LogTemp, Warning, TEXT("[RangedAttack] PerformRangedAttack called, but ProjectileClass is not set yet."));

	// 공격 종료 처리 (애니메이션 종료 시 호출할 수도 있음)
	StopAttack();
}