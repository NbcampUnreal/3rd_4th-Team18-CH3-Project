#include "Room/Public/Subsystem/StaticDataSubsystem.h"
#include "StaticData/StaticDataStruct.h"

void UStaticDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    for (TObjectPtr<UDataTable> DataTable : DataTablesToRegister)
    {
        if (DataTable)
        {
            FName DataTableName = DataTable->GetFName();
            if (!DataTables.Contains(DataTableName))
            {
                DataTables.Add(DataTableName, DataTable);
            }
        }
    }

    RegisterDataManager<FItemData>(TEXT("ItemDataTable"), [](const FItemData& Data) { return Data.ID; });
    RegisterDataManager<FInventoryData>(TEXT("InventoryDataTable"), [](const FInventoryData& Data) { return Data.ID; });
    RegisterDataManager<FWeaponData>(TEXT("WeaponDataTable"), [](const FWeaponData& Data) { return Data.ID; });
    RegisterDataManager<FBulletData>(TEXT("BulletDataTable"), [](const FBulletData& Data) { return Data.ID; });
    RegisterDataManager<FEnemyData>(TEXT("EnemyDataTable"), [](const FEnemyData& Data) { return Data.ID; });
    RegisterDataManager<FMapSpawnData>(TEXT("MapSpawnDataTable"), [](const FMapSpawnData& Data) { return Data.ID; });
}

void UStaticDataSubsystem::Deinitialize()
{
    StaticDataManagers.Empty();
    Super::Deinitialize();
}
