#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::SetDestroyDelay(float NewDelay)
{
	if (NewDelay<=0.0f) NewDelay = 1.0f;
	DestroyDelay = NewDelay;
}

void UHealthComponent::SetMaxHealth(float NewMaxHealth)
{
	if (NewMaxHealth<=0.0f) NewMaxHealth = 1.0f;
	MaxHealth = NewMaxHealth;
}

float UHealthComponent::GetDestroyDelay() const
{
	return DestroyDelay;	
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;

	if (AActor* Owner = GetOwner())
	{
		Owner->OnTakeAnyDamage.AddDynamic(this,&ThisClass::DamageTaken);
	}
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamagedActor != CharacterRef) return;

	SetCurrentHealth(CurrentHealth - Damage);
}


void UHealthComponent::SetCurrentHealth(float NewHealth)
{
	CurrentHealth = NewHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		OnDead.Execute();
	}
}

