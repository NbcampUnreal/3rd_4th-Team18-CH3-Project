
#include "ItemSystem/Item/Bullet/BulletItem.h"

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

bool UBulletItem::IsUsable_Implementation() const
{
	return BulletItemInfo.bUsable;
}

float UBulletItem::GetInitialSpeed() const
{
	return BulletItemInfo.BulletInitialSpeed;
}

float UBulletItem::GetMaxSpeed() const
{
	return BulletItemInfo.BulletMaxSpeed;
}

float UBulletItem::GetAutoDestroyTime() const
{
	return BulletItemInfo.BulletAutoDestroyTime;
}

int32 UBulletItem::GetBulletID() const
{
	return BulletItemInfo.BulletID;
}

void UBulletItem::SetBulletItemInfo(const FBulletItemInfo& BulletInfo)
{
	BulletItemInfo = BulletInfo;
}
