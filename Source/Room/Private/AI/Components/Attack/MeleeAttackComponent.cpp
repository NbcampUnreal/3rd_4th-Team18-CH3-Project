// MeleeAttackComponent.cpp
#include "AI/Components/Attack/MeleeAttackComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

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

	// 소유한 캐릭터의 애니메이션 인스턴스 가져오기
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && AttackMontage)
	{
		// 메시에서 AnimInstance 가져오기
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// DEBUG : 애니메이션 인스턴스 및 몽타주 정보
			//UE_LOG(LogTemp, Warning, TEXT("[AI][MeleeAttack][DEBUG] %s: AnimInstance Address=%p, Montage=%s"),
			//	*GetOwner()->GetName(), AnimInstance, *AttackMontage->GetName());

			if (AnimInstance->IsAnyMontagePlaying())
			{// 다른 몽타주가 재생 중이라면 로그 출력
				UE_LOG(LogTemp, Warning, TEXT("[AI][MeleeAttack][DEBUG] %s: Other montage already playing!"), *GetOwner()->GetName());
			}
			/*
				Montage_Play : 내부적으로 기본 슬롯(DefaultSlot)로 재생
				슬롯을 따로 구성해도 반영되지 않을 수 있음.
			*/
			// 애니메이션 몽타주 재생(슬롯 설정 무시 가능성 있음)
			AnimInstance->Montage_Play(AttackMontage);

			if (AnimInstance->Montage_IsPlaying(AttackMontage))
			{// 재생 성공 여부 확인
				//UE_LOG(LogTemp, Warning, TEXT("[AI][MeleeAttack][DEBUG] %s: Montage successfully started"), *GetOwner()->GetName());
			}
			//UE_LOG(LogTemp, Log, TEXT("[AI][MeleeAttack] Montage Played for %s"), *GetOwner()->GetName());
		}
	}
	else
	{// Owner 또는 AttackMontage가 유효하지 않은 경우 경고 로그
		UE_LOG(LogTemp, Warning, TEXT("[AI][MeleeAttack] Cannot play montage: Owner or Montage invalid"));
	}
}

void UMeleeAttackComponent::StopAttack()
{
	// 공격 상태 종료
	bIsAttacking = false;

	// 애니메이션 정지 처리
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && AttackMontage)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && AnimInstance->Montage_IsPlaying(AttackMontage))
		{
			// 부드럽게 정지 (0.25초 페이드 아웃)
			AnimInstance->Montage_Stop(0.25f, AttackMontage);
			//UE_LOG(LogTemp, Log, TEXT("[AI][MeleeAttack] Montage stopped for %s"), *GetOwner()->GetName());
		}
	}

	//UE_LOG(LogTemp, Log, TEXT("[AI][MeleeAttack] StopAttack called for %s"), *GetOwner()->GetName());
}

float UMeleeAttackComponent::GetAttackDuration() const
{
	// 애니메이션 몽타주가 유효하면 길이 반환, 없으면 기본 1초
	return AttackMontage ? AttackMontage->GetPlayLength() : 1.0f;
}

void UMeleeAttackComponent::PerformMeleeAttack()
{
	// TODO: 공격 로직 구현

	// 공격자 유효성 확인
	if (!GetOwner()) return;

	// 공격 시작 위치와 방향 계산
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * AttackData.AttackRange;

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
				UGameplayStatics::ApplyDamage(HitActor, AttackData.AttackDamage, nullptr, GetOwner(), UDamageType::StaticClass());
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
