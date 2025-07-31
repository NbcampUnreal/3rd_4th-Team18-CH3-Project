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

    
    RegisterDataManager<FItemData>(GetDTName<FItemData>());
    RegisterDataManager<FInventoryData>(GetDTName<FInventoryData>());
    RegisterDataManager<FBulletData>(GetDTName<FBulletData>());
    RegisterDataManager<FWeaponData>(GetDTName<FWeaponData>());
    RegisterDataManager<FEnemyData>(GetDTName<FEnemyData>());
   
}

void UStaticDataSubsystem::Deinitialize()
{
    
    StaticDataManagers.Empty();
    Super::Deinitialize();
}



