#include "GameInstance/StaticDataSubsystem.h"

void UStaticDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

const FItemData* UStaticDataSubsystem::GetItemData(int32 ItemID) const
{
	return ItemDataMap.Find(ItemID);
}

const FWeaponData* UStaticDataSubsystem::GetWeaponData(int32 WeaponID) const
{
	return WeaponDataMap.Find(WeaponID);
}

const FBulletData* UStaticDataSubsystem::GetBulletData(int32 BulletID) const
{
	return BulletDataMap.Find(BulletID);
}

const FMonsterData* UStaticDataSubsystem::GetMonsterData(int32 MonsterID) const
{
	return MonsterDataMap.Find(MonsterID);
}