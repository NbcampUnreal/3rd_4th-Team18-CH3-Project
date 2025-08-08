#include "Components/PlayerAttackComponent.h"


UPlayerAttackComponent::UPlayerAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bIsFiring = false;
}



void UPlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWorld() is nullptr"));
		return;
	}

	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance is nullptr"));
		return;
	}

	UStaticDataSubsystem* StaticData = GameInstance->GetSubsystem<UStaticDataSubsystem>();
	if (!StaticData)
	{
		UE_LOG(LogTemp, Warning, TEXT("StaticDataSubsystem is nullptr"));
		return;
	}

	const FWeaponData* WeaponData = StaticData->GetData<FWeaponData>(CurrentWeaponID);
	if (!WeaponData)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponData not found for ID %d"), CurrentWeaponID);
		return;
	}

	Damage = WeaponData->Damage;
	FireRate = WeaponData->FireRate;
	Spread = WeaponData->Spread;
	BulletID = WeaponData->BulletID;


	UE_LOG(LogTemp, Warning, TEXT("Weapon ID: %d, Damage: %f, FireRate: %f"), CurrentWeaponID, Damage, FireRate);
}

void UPlayerAttackComponent::StartFire()
{
	if (FireRate <= 0.0f)
	{
		Fire(); 
		return;
	}
	if (!bIsFiring)
	{
		bIsFiring = true;
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UPlayerAttackComponent::Fire, FireRate, true, 0.0f);
	}
}

void UPlayerAttackComponent::StopFire()
{
	if (bIsFiring)
	{
		bIsFiring = false;
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}
}


void UPlayerAttackComponent::Fire()
{
	UE_LOG(LogTemp, Log, TEXT("Fire!"));
	OnFire.Broadcast();
	// TODO: 탄환 액터 발사 로직 추가 
}
