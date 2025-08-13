#include "Actor/Character/BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HealthComponent)
	{
		HealthComponent->OnDead.AddDynamic(this, &ABaseCharacter::HandleDeath);
		HealthComponent->OnHit.AddDynamic(this, &ABaseCharacter::HandleHit);
	}
}

void ABaseCharacter::HandleDeath()
{
	bIsDead = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->DisableMovement();   // 이동 정지
	}
	
	//컨트롤러 해제 등은 상속받은 캐릭터에서 처리
}

void ABaseCharacter::HandleHit()
{
	RunMontage(ECharacterAnim::GetHit);
}

void ABaseCharacter::RunMontage(ECharacterAnim Anim)
{
	if (AnimMontages.Contains(Anim))
	{
		UAnimMontage* MontageToPlay = AnimMontages[Anim];

		// 애니메이션 몽타주가 유효 시
		if (MontageToPlay && GetMesh()->GetAnimInstance())
		{
			// 애니메이션 몽타주를 재생
			GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);

			// 현재 애니메이션 상태를 업데이트
			CurrentAnimState = Anim;

		}
	}
}

bool ABaseCharacter::StopMontage()
{
	if (GetMesh()->GetAnimInstance() && GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		// 모든 몽타주를 중지
		GetMesh()->GetAnimInstance()->Montage_Stop(0.2f);
        
		// 애니메이션 상태 초기화
		CurrentAnimState = ECharacterAnim::Idle; 

		return true;
	}
	return false;
}

ECharacterAnim ABaseCharacter::GetCurrentCharacterAnim() const
{
	return CurrentAnimState;
}

float ABaseCharacter::GetMontagePlayLength(ECharacterAnim AnimType) const
{
	if (const TObjectPtr<UAnimMontage>* MontagePtr = AnimMontages.Find(AnimType))
	{
		if (*MontagePtr)
		{
			return (*MontagePtr)->GetPlayLength();
		}
	}
	return 1.0f; // 기본 fallback 값
}

void ABaseCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(OwnedGameplayTags);
}
