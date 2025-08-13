// MeleeAttackComponent.cpp
#include "AI/Components/Attack/MeleeAttackComponent.h"

#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Define/GameDefine.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/MeleeEnemyCharacter.h"

UMeleeAttackComponent::UMeleeAttackComponent()
{
	// 기본 근접 공격 세팅 초기화
	AttackData.AttackRange = 200.f;		// 최대 도달 거리: 200cm (2미터)
	AttackData.AttackRadius = 50.f;		// 공격 범위 반경: 50cm (0.5미터)
	AttackData.AttackDamage = 20.f;		// 기본 데미지
	AttackData.AttackCooldown = 1.5f;	// 공격 쿨타임 (1.5초)
}

/*
	StartAttack() 호출

	1. 공격 가능 여부 확인 (쿨타임 등)
	2. 공격 상태 활성화 + 마지막 공격 시간 기록
	3. 애니메이션 몽타주 재생
	4. AnimNotify(Montage 내 특정 프레임)에서 실제 공격 처리 함수 호출
	   - 근거리: 데미지 적용
	   - 원거리: 투사체 스폰
	5. 애니메이션 종료 -> StopAttack() 호출 (bIsAttacking = false, 후처리)
*/
void UMeleeAttackComponent::StartAttack()
{
	// 공격이 가능한 상태인지 확인 (쿨타임, 공격 중 여부 등)
	if (!CanAttack())
		return;

	// Base 클래스의 공통 처리 (상태 활성화, 타이머 기록 등)
	// - bIsAttacking = true
	// - LastAttackTime = 현재 시간
	Super::StartAttack();

	// Owner를 ACharacter로 캐스팅
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	// Owner가 근접 AI 캐릭터인 경우
	AMeleeEnemyCharacter* MeleeCharacter = Cast<AMeleeEnemyCharacter>(OwnerCharacter);
	if (MeleeCharacter)
	{// 공격 애니메이션 몽타주 실행
		MeleeCharacter->RunMontage(ECharacterAnim::Attacking);
	}
}

void UMeleeAttackComponent::StopAttack()
{
	// 공격 상태 비활성화
	bIsAttacking = false;

	AMeleeEnemyCharacter* OwnerCharacter = Cast<AMeleeEnemyCharacter>(GetOwner());
	if (OwnerCharacter)
	{// 재생 중인 애니메이션 몽타주 중지
		OwnerCharacter->StopMontage();
	}
}

void UMeleeAttackComponent::PerformMeleeAttack()
{
	// 공격자 유효성 확인
	if (!GetOwner()) return;

	// 공격 시작 위치와 방향 계산
	FVector Start = GetComponentLocation();
	FVector Forward = GetForwardVector();
	FVector End = Start + Forward * AttackData.AttackRange;
	// 구형 히트 박스 생성
	TArray<FHitResult> HitResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AttackData.AttackRadius);
	
	
	// 충돌 판정 (멀티 히트 가능)
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_Pawn,
		CollisionShape
	);

	// 충돌한 액터들에 데미지 적용
	if (bHit)
	{
		for (auto& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor != GetOwner())
			{
				// IGameplayTagAssetInterface를 사용하여 태그 감지
				if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(HitActor))
				{
					// HitActor가 플레이어 태그를 가지고 있는지 확인
					if (TagInterface->HasMatchingGameplayTag(GameDefine::PlayerTag))
					{
						UE_LOG(LogTemp, Warning, TEXT("[AI][MeleeAttack] Applying damage to Player: %s"), *HitActor->GetName());
						UGameplayStatics::ApplyDamage(HitActor, AttackData.AttackDamage, nullptr, GetOwner(), UDamageType::StaticClass());
					}
				}
			}
		}
	}

#if WITH_EDITOR
	// DEBUG: 공격 영역 시각화
	DrawDebugSphere(GetWorld(), Start, AttackData.AttackRadius, 16, FColor::Red, false, 1.0f);
#endif

	// 공격 종료 처리
	StopAttack();
}
