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
    //신규 데이터 테이블 추가시 매니저 등록 함수도 추가
    RegisterDataManager<FItemData>(GetDTName<FItemData>());
    RegisterDataManager<FInventoryData>(GetDTName<FInventoryData>());
    RegisterDataManager<FBulletData>(GetDTName<FBulletData>());
    RegisterDataManager<FWeaponData>(GetDTName<FWeaponData>());
    RegisterDataManager<FEnemyData>(GetDTName<FEnemyData>());
    RegisterDataManager<FRoomData>(GetDTName<FRoomData>());
}

void UStaticDataSubsystem::Deinitialize()
{
    
    StaticDataManagers.Empty();
    Super::Deinitialize();
}



