#include "Room/Public/Subsystem/StaticDataSubsystem.h"

#include "Engine/AssetManager.h"

#include "Engine/AssetManager.h"
#include "StaticData/StaticDataStruct.h"
#include "Subsystem/ObjectPoolSubsystem.h"

void UStaticDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    // Primary Asset ID: TypeName, AssetName
    TArray<FPrimaryAssetId> AssetIds;
    UAssetManager::Get().GetPrimaryAssetIdList(FPrimaryAssetType(TEXT("StaticDataTable")), AssetIds);

    for (const FPrimaryAssetId& Id : AssetIds)
    {
        FSoftObjectPath AssetPath = UAssetManager::Get().GetPrimaryAssetPath(Id);
        if (UDataTable* Table = Cast<UDataTable>(AssetPath.TryLoad()))
        {
            DataTables.Add(Table->GetFName(), Table);
            UE_LOG(LogTemp, Log, TEXT("Loaded Primary Asset DataTable: %s"), *Table->GetName());
        }
    }
    //신규 데이터 테이블 추가시 매니저 등록 함수도 추가
    RegisterDataManager<FItemData>(GetDTName<FItemData>());
    RegisterDataManager<FInventoryData>(GetDTName<FInventoryData>());
    RegisterDataManager<FWeaponItemData>(GetDTName<FWeaponItemData>());
    RegisterDataManager<FHealingItemData>(GetDTName<FHealingItemData>());
    RegisterDataManager<FBulletItemData>(GetDTName<FBulletItemData>());
    RegisterDataManager<FEnemyData>(GetDTName<FEnemyData>());
    RegisterDataManager<FRoomData>(GetDTName<FRoomData>());
    RegisterDataManager<FUIData>(GetDTName<FUIData>());
    RegisterDataManager<FSoundData>(GetDTName<FSoundData>());
    RegisterDataManager<FStatData>(GetDTName<FStatData>());

    RegisterExtraKey<FBulletItemData, int32>([](const FBulletItemData& Data)
    {
        return Data.ID;
    });
    // FRoomData에 대한 보조 인덱스 등록 예시 (LevelName을 키로 사용)
    RegisterExtraKey<FRoomData, FName>([](const FRoomData& Data) {
        return FName(Data.Level.ToSoftObjectPath().GetAssetName());
    });
}

void UStaticDataSubsystem::Deinitialize()
{
    StaticDataManagers.Empty();
    Super::Deinitialize();
}