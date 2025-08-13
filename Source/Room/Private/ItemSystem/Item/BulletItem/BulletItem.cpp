// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSystem/Item/BulletItem/BulletItem.h"

void UBulletItem::Use_Implementation(AActor* User)
{
	Super::Use_Implementation(User);
}

FName UBulletItem::GetItemID_Implementation() const
{
	return BulletItemInfo.ItemID;
}

FName UBulletItem::GetItemName_Implementation() const
{
	return BulletItemInfo.ItemName;
}

FText UBulletItem::GetItemDescription_Implementation() const
{
	return BulletItemInfo.ItemDescription;
}

UTexture2D* UBulletItem::GetItemIcon_Implementation() const
{
	return BulletItemInfo.ItemIcon;
}

UStaticMesh* UBulletItem::GetItemMesh_Implementation() const
{
	return BulletItemInfo.ItemMesh;
}

bool UBulletItem::IsConsumable_Implementation() const
{
	return BulletItemInfo.bConsumable;
}

int32 UBulletItem::GetMaxStackCount_Implementation() const
{
	return BulletItemInfo.MaxStackCount;
}

int32 UBulletItem::GetBulletID() const
{
	return BulletItemInfo.BulletID;
}

float UBulletItem::GetBulletInitialSpeed() const
{
	return BulletItemInfo.BulletInitialSpeed;
}

float UBulletItem::GetBulletMaxSpeed() const
{
	return BulletItemInfo.BulletMaxSpeed;
}

float UBulletItem::GetBulletAutoDestroyTime() const
{
	return BulletItemInfo.BulletAutoDestroyTime;
}

void UBulletItem::SetBulletItemInfo(const FBulletItemInfo& _BulletItemInfo)
{
	BulletItemInfo = _BulletItemInfo;
}
