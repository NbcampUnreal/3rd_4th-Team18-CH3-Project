
#include "Components/WeaponComponent.h"

#include "Actor/Projectile/BaseProjectile.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ItemSystem/InventoryComponent/InventoryComponent.h"
#include "ItemSystem/Item/WeaponItem/WeaponItem.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/ObjectPoolSubsystem.h"
#include "Subsystem/StaticDataSubsystem.h"

UWeaponComponent::UWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	InventoryRef = UGameplayStatics::GetPlayerState(this,0)->FindComponentByClass<UInventoryComponent>();
}

void UWeaponComponent::Equip(UWeaponItem* NewItem)
{
	if (bIsPlayerWeaponComp && InventoryRef.IsValid())
	{
		if (IsValid(Weapon))
		{
			InventoryRef->AddItemToInventory(Weapon,1);
		}
	}
	
	Weapon = NewItem;
}

void UWeaponComponent::UnEquip()
{
	if (bIsPlayerWeaponComp)
	{
		if (GetOwner() == UGameplayStatics::GetPlayerPawn(this,0))
		{
			auto PlayerState = UGameplayStatics::GetPlayerState(this,0);
			auto Inventory = PlayerState->FindComponentByClass<UInventoryComponent>();
			Inventory->AddItemToInventory(Weapon,1);
		}
	}
	
	Weapon = nullptr;
}

void UWeaponComponent::Fire()
{
	if (!Weapon)
	{
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("무기가 장착되지 않음."));
		return;
	}
	bool UseBulletSuccess = InventoryRef->UseBulletForWeaponFire(Weapon->GetWeaponBulletID());
	if (UseBulletSuccess)
	{
		FTransform SpawnTransform = GetComponentTransform();
		//
		AActor* SpawnedActor = GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->GetPooledObject(ProjectileClass, SpawnTransform);
		if (ABaseProjectile* Projectile = Cast<ABaseProjectile>(SpawnedActor))
		{
			Projectile->Shooter = GetOwner(); 
		}
		//
		UStaticDataSubsystem* StaticDataSys = GetWorld()->GetGameInstance()->GetSubsystem<UStaticDataSubsystem>();
		if (auto Data =StaticDataSys->GetDataByKey<FBulletItemData, int32>(Weapon->GetWeaponBulletID()))
		{
			IProjectileDataReciever::Execute_SetProjectileMoveData(SpawnedActor, *Data);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Cyan,TEXT("데이터 로딩 실패"));
		}
	}
	else
	{
		
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("탄알이 부족함."));
	}
}
