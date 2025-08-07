#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::SetMaxHealth(float NewMaxHealth)
{
	if (NewMaxHealth<=0.0f) NewMaxHealth = 1.0f;
	MaxHealth = NewMaxHealth;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//CurrentHealth = MaxHealth;
	SetCurrentHealth(CurrentHealth); 
    
    
	if (AActor* Owner = GetOwner())
	{
		Owner->OnTakeAnyDamage.AddDynamic(this,&ThisClass::DamageTaken);
	}
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	SetCurrentHealth(CurrentHealth - Damage);
}


void UHealthComponent::SetCurrentHealth(float NewHealth)
{
	CurrentHealth = NewHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		OnDead.Broadcast();
	}
}

