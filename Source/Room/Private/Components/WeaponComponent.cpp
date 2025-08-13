
#include "Components/WeaponComponent.h"

#include "GameFramework/PlayerState.h"
#include "ItemSystem/InventoryComponent/InventoryComponent.h"
#include "ItemSystem/Item/WeaponItem/WeaponItem.h"
#include "Kismet/GameplayStatics.h"

UWeaponComponent::UWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UWeaponComponent::Equip(UWeaponItem* NewItem)
{
	Weapon = NewItem;
}

void UWeaponComponent::UnEquip()
{
	if (GetOwner() == UGameplayStatics::GetPlayerPawn(this,0))
	{
		auto PlayerState = UGameplayStatics::GetPlayerState(this,0);
		auto Inventory = PlayerState->FindComponentByClass<UInventoryComponent>();
		Inventory->AddItemToInventory(Weapon,1);
	}
	Weapon = nullptr;
}

void UWeaponComponent::Fire()
{
	// 스폰 프로젝타일.
}
