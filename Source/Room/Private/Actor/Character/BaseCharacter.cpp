#include "Actor/Character/BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HealthComponent)
	{
		HealthComponent->OnDead.BindUObject(this, &ABaseCharacter::HandleDeath);
	}
}

void ABaseCharacter::HandleDeath()
{
	//캐릭터 사망처리
}

