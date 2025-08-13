// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSystem/Item/WeaponItem/WeaponItem.h"

#include "Components/WeaponComponent.h"
#include "ItemSystem/Interfaces/Equipper.h"

void UWeaponItem::Use_Implementation(AActor* User)
{
	Super::Use_Implementation(User);
	// if (User->GetClass()->ImplementsInterface(UEquipper::StaticClass()))
	// {
	// 	IEquipper::Execute_Equip(User, this);
	// }
	if (UWeaponComponent* WeaponComponent = User->FindComponentByClass<UWeaponComponent>())
	{
		WeaponComponent->Equip(this);
	}
}

FName UWeaponItem::GetItemID_Implementation() const
{
	return WeaponItemInfo.ItemID;
}

FName UWeaponItem::GetItemName_Implementation() const
{
	return WeaponItemInfo.ItemName;
}

FText UWeaponItem::GetItemDescription_Implementation() const
{
	return WeaponItemInfo.ItemDescription;
}

UTexture2D* UWeaponItem::GetItemIcon_Implementation() const
{
	return WeaponItemInfo.ItemIcon;
}

UStaticMesh* UWeaponItem::GetItemMesh_Implementation() const
{
	return WeaponItemInfo.ItemMesh;
}

bool UWeaponItem::IsConsumable_Implementation() const
{
	return WeaponItemInfo.bConsumable;
}

int32 UWeaponItem::GetMaxStackCount_Implementation() const
{
	return WeaponItemInfo.MaxStackCount;
}

float UWeaponItem::GetWeaponAttackDamage() const
{
	return WeaponItemInfo.WeaponAttackDamage;
}

float UWeaponItem::GetWeaponAttackSpeed() const
{
	return WeaponItemInfo.WeaponAttackSpeed;
}

float UWeaponItem::GetWeaponAttackRecoil() const
{
	return WeaponItemInfo.WeaponRecoil;
}

float UWeaponItem::GetWeaponAttackRange() const
{
	return WeaponItemInfo.WeaponAttackRange;
}

int32 UWeaponItem::GetrWeaponBulletID() const
{
	return WeaponItemInfo.WeaponBulletID;
}

void UWeaponItem::SetWeaponItemInfo(const FWeaponItemInfo& WeaponInfo)
{
	WeaponItemInfo = WeaponInfo;
}
