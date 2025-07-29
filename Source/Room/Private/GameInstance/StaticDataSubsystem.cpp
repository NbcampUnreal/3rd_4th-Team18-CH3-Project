#include "GameInstance/StaticDataSubsystem.h"

void UStaticDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

  //로드
    LoadItemData();
    LoadInventoryData();
    LoadWeaponData();
    LoadBulletData();
    LoadEnemyData();
    LoadMapSpawnData();

}

void UStaticDataSubsystem::Deinitialize()
{
    //언로드
    ItemDataMap.Empty();
    WeaponDataMap.Empty();
    BulletDataMap.Empty();
    EnemyDataMap.Empty();
    MapSpawnDataMap.Empty();
  
    InventoryData = FInventoryData(); 
   
    ItemDataTable = nullptr;
    InventoryDataTable = nullptr;
    WeaponDataTable = nullptr;
    BulletDataTable = nullptr;
    EnemyDataTable = nullptr;
    MapSpawnDataTable = nullptr;
   

    Super::Deinitialize();
}

void UStaticDataSubsystem::LoadItemData()
{
    if (!ItemDataTable)
    {
        return;
    }

    TArray<FItemData*> Rows;
    ItemDataTable->GetAllRows<FItemData>(TEXT("LoadItemData"), Rows);

    ItemDataMap.Empty();
    for (FItemData* Row : Rows)
    {
        if (Row)
        {
            ItemDataMap.Add(Row->ItemID, *Row);
        }
    }
  
}

void UStaticDataSubsystem::LoadInventoryData()
{
    if (!InventoryDataTable)
    {
        return;
    }
    //행 이름이 DefaultInventoryData일 때만 데이터를 받음  
    FName DefaultInventoryRowName = TEXT("DefaultInventoryData"); 
    
    FInventoryData* FoundRow = InventoryDataTable->FindRow<FInventoryData>(DefaultInventoryRowName, TEXT("LoadInventoryData"));

    if (FoundRow)
    {
        InventoryData = *FoundRow;
    }
    else
    {
        InventoryData = FInventoryData();
    }
}

void UStaticDataSubsystem::LoadWeaponData()
{
    if (!WeaponDataTable)
    {
        return;
    }

    TArray<FWeaponData*> Rows;
    WeaponDataTable->GetAllRows<FWeaponData>(TEXT("LoadWeaponData"), Rows);

    WeaponDataMap.Empty();
    for (FWeaponData* Row : Rows)
    {
        if (Row)
        {
            WeaponDataMap.Add(Row->WeaponID, *Row);
        }
    }
}

void UStaticDataSubsystem::LoadBulletData()
{
    if (!BulletDataTable)
    {
        return;
    }

    TArray<FBulletData*> Rows;
    BulletDataTable->GetAllRows<FBulletData>(TEXT("LoadBulletData"), Rows);

    BulletDataMap.Empty();
    for (FBulletData* Row : Rows)
    {
        if (Row)
        {
            BulletDataMap.Add(Row->BulletID, *Row);
        }
    }
}

void UStaticDataSubsystem::LoadEnemyData()
{
    if (!EnemyDataTable)
    {
        return;
    }

    TArray<FEnemyData*> Rows;
    EnemyDataTable->GetAllRows<FEnemyData>(TEXT("LoadEnemyData"), Rows);

    EnemyDataMap.Empty();
    for (FEnemyData* Row : Rows)
    {
        if (Row)
        {
            EnemyDataMap.Add(Row->EnemyID, *Row);
        }
    }
}

void UStaticDataSubsystem::LoadMapSpawnData()
{
    if (!MapSpawnDataTable)
    {
        return;
    }

    TArray<FMapSpawnData*> Rows;
    MapSpawnDataTable->GetAllRows<FMapSpawnData>(TEXT("LoadMapSpawnData"), Rows);

    MapSpawnDataMap.Empty();
    for (FMapSpawnData* Row : Rows)
    {
        if (Row && !Row->MapName.IsNone())
        {
            MapSpawnDataMap.Add(Row->MapName, *Row);
        }
      
    }
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