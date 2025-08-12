#include "Actor/Character/BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ABaseCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(OwnedGameplayTags);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseCharacter::HandleDeath()
{
	bIsDead = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	//컨트롤러 해제 등은 상속받은 캐릭터에서 처리
}

void ABaseCharacter::RunMontage(ECharacterAnim Anim)
{
	if (AnimMontages.Contains(Anim))
	{
		UAnimMontage* MontageToPlay = AnimMontages[Anim];
        
		// 애니메이션 몽타주가 유효하고, 이미 재생 중인 애니메이션이 아닐 경우
		if (MontageToPlay && GetMesh()->GetAnimInstance() && !GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
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

