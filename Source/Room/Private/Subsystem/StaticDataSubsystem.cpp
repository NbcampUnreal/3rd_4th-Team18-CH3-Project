#include "Room/Public/Subsystem/StaticDataSubsystem.h"
#include "StaticData/StaticDataStruct.h"
#include "Subsystem/ObjectPoolSubsystem.h"

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
    RegisterDataManager<FUIData>(GetDTName<FUIData>());

    // FRoomData에 대한 보조 인덱스 등록 예시 (LevelName을 키로 사용)
    RegisterExtraKey<FRoomData, FName>([](const FRoomData& Data) {
        return Data.Level->GetFName();
    });
}

void UStaticDataSubsystem::Deinitialize()
{
    StaticDataManagers.Empty();
    Super::Deinitialize();
}