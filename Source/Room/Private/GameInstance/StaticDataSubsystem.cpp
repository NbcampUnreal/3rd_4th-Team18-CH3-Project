#include "GameInstance/StaticDataSubsystem.h"

void UStaticDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}


const FItemData* UStaticDataSubsystem::GetItemData(int32 ItemID) const
{
	return ItemDataMap.Find(ItemID);
}

const FInventoryData& UStaticDataSubsystem::GetInventoryData() const
{
	return InventoryData;
}


const FWeaponData* UStaticDataSubsystem::GetWeaponData(int32 WeaponID) const
{
	return WeaponDataMap.Find(WeaponID);
}

const FBulletData* UStaticDataSubsystem::GetBulletData(int32 BulletID) const
{
	return BulletDataMap.Find(BulletID);
}

const FEnemyData* UStaticDataSubsystem::GetEnemyData(int32 EnemyID) const
{
	return EnemyDataMap.Find(EnemyID);
}

const FMapSpawnData* UStaticDataSubsystem::GetMapSpawnData(FName MapName) const
{
	return MapSpawnDataMap.Find(MapName);
}

