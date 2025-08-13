#include "Subsystem/LoadingSubsystem.h"

#include "Core/GameManager.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "StaticData/EnemyData.h"
#include "StaticData/GameConfigData.h"
#include "StaticData/RoomData.h"
#include "Subsystem/StaticDataSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LoadingSubsystem)


void ULoadingSubsystem::RequestLoad(const TSoftObjectPtr<UObject>& Asset, FOnAssetLoadComplete OnComplete)
{
    if (!Asset.IsValid() && Asset.ToSoftObjectPath().IsNull())
    {
        OnComplete.ExecuteIfBound();
        return;
    }

    FSoftObjectPath Path = Asset.ToSoftObjectPath();
    AssetRefCount.FindOrAdd(Path)++;
    
    if (LoadedAssets.Contains(Path))
    {
        // 이미 로드됨
        OnComplete.ExecuteIfBound();
        return;
    }

    // 이미 로드 중인 경우: 핸들 존재 여부로 체크
    if (ActiveHandles.Contains(Path))
    {
        // 이미 로드 중이므로 새 콜백은 추가하지 않음
        return;
    }

    TSharedPtr<FStreamableHandle> Handle = StreamableManager.RequestAsyncLoad(
        Path,
        FStreamableDelegate::CreateLambda([this, Path, OnComplete]()
    {
        LoadedAssets.Add(Path);
        ActiveHandles.Remove(Path);
        OnComplete.ExecuteIfBound();
    })
    );

    if (Handle.IsValid())
    {
        ActiveHandles.Add(Path, Handle);
    }
}

void ULoadingSubsystem::RequestLoadBatch(const TArray<TSoftObjectPtr<UObject>>& Assets, FOnAssetLoadComplete OnComplete)
{
    TArray<FSoftObjectPath> PathsToLoad;

    for (const TSoftObjectPtr<UObject>& Asset : Assets)
    {
        if (!Asset.IsValid())
            continue;

        FSoftObjectPath Path = Asset.ToSoftObjectPath();
        AssetRefCount.FindOrAdd(Path)++;

        if (!LoadedAssets.Contains(Path) && !ActiveHandles.Contains(Path))
        {
            PathsToLoad.Add(Path);
        }
    }

    if (PathsToLoad.Num() == 0)
    {
        OnComplete.ExecuteIfBound();
        return;
    }

    TSharedPtr<FStreamableHandle> Handle = StreamableManager.RequestAsyncLoad(
        PathsToLoad,
        FStreamableDelegate::CreateLambda([this, PathsToLoad, OnComplete]()
        {
            for (const FSoftObjectPath& Path : PathsToLoad)
            {
                LoadedAssets.Add(Path);
                ActiveHandles.Remove(Path);
            }
            OnComplete.ExecuteIfBound();
        })
    );

    for (const FSoftObjectPath& Path : PathsToLoad)
    {
        ActiveHandles.Add(Path, Handle);
    }
}

void ULoadingSubsystem::RequestUnload(const TSoftObjectPtr<UObject>& Asset)
{
    FSoftObjectPath Path = Asset.ToSoftObjectPath();
    RequestUnload(Path);
}

void ULoadingSubsystem::RequestUnload(const FSoftObjectPath Path)
{
    if (int32* Count = AssetRefCount.Find(Path))
    {
        (*Count)--;
        if (*Count <= 0)
        {
            AssetRefCount.Remove(Path);
            if (LoadedAssets.Contains(Path))
            {
                StreamableManager.Unload(Path);
                LoadedAssets.Remove(Path);
            }
        }
    }
}


bool ULoadingSubsystem::IsLoaded(const TSoftObjectPtr<UObject>& Asset) const
{
    return LoadedAssets.Contains(Asset.ToSoftObjectPath());
}

void ULoadingSubsystem::UnloadUnusedAssets(const TSet<FSoftObjectPath>& KeepList)
{
    for (auto It = AssetRefCount.CreateIterator(); It; ++It)
    {
        if (!KeepList.Contains(It->Key))
        {
            StreamableManager.Unload(It->Key);
            LoadedAssets.Remove(It->Key);
            It.RemoveCurrent();
        }
    }
    CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
}

void ULoadingSubsystem::GetOrLoadAsset(const TSoftObjectPtr<UObject>& Asset, FOnAssetReady OnReady)
{
    if (Asset.IsValid())
    {
        // 이미 메모리에 있음 → 바로 반환
        OnReady.ExecuteIfBound(Asset.Get());
        return;
    }

    if (Asset.ToSoftObjectPath().IsNull())
    {
        OnReady.ExecuteIfBound(nullptr);
        return;
    }

    // 비동기 로드
    RequestLoad(Asset, FOnAssetLoadComplete::CreateLambda([Asset, OnReady]()
    {
        OnReady.ExecuteIfBound(Asset.Get());
    }));
}

float ULoadingSubsystem::GetLoadingProgress() const
{
    if (ActiveHandles.Num() == 0)
    {
        return -1.0f;
    }

    float TotalProgress = 0.0f;
    int32 HandleCount = 0;

    for (const auto& Pair : ActiveHandles)
    {
        if (Pair.Value.IsValid())
        {
            TotalProgress += Pair.Value->GetProgress();
            HandleCount++;
        }
        else TotalProgress += 1.f;
    }

    return TotalProgress / ActiveHandles.Num();
}

TArray<FSoftObjectPath> ULoadingSubsystem::GetRoomDataNeedSoftPaths(const FRoomData& NewRoomData) const
{
    TArray<FSoftObjectPath> Array;
    Array.Reserve(
        (NewRoomData.PreloadAssets.IsEmpty() ? 0 : NewRoomData.PreloadAssets.Num()) +
        (NewRoomData.Monsters.IsEmpty() ? 0 : NewRoomData.Monsters.Num() * 2)
        //(NewRoomData.Items.IsEmpty() ? 0 : NewRoomData.Items.Num())
        );
    
    for (const TSoftObjectPtr<UObject>& Path : NewRoomData.PreloadAssets)
    {
        Array.Add(Path.ToSoftObjectPath());
    }
    // 몬스터 데이터 추가.
    UStaticDataSubsystem* StaticSys = GetGameManager()->GetSubsystem<UStaticDataSubsystem>();

    for (const FDataTableRowHandle MonsterDataHandle : NewRoomData.Monsters)
    {
        FEnemyData* EnemyData = MonsterDataHandle.GetRow<FEnemyData>(TEXT("Not Found"));
        Array.Add(EnemyData->Behavior.ToSoftObjectPath());
        Array.Add(EnemyData->EnemyClass.ToSoftObjectPath());
    }

    // for (const FDataTableRowHandle ItemHandle : NewRoomData.Items)
    // {
    //     FItemData* ItemData = ItemHandle.GetRow<FItemData>(TEXT("Not Found"));
    //     Array.Add(ItemData->ItemBPClass.ToSoftObjectPath());
    // }

    return Array;
}

void ULoadingSubsystem::LoadLevelWithLoadingScreen(const FRoomData& NewRoomData)
{
    PendingTargetLevel = NewRoomData.Level;
    PendingResourcesPath.Empty();
    PendingResourcesPath = GetRoomDataNeedSoftPaths(NewRoomData);
    
    // 1. GameConfig에서 로딩 레벨 경로 가져오기
    const auto GameConfig = UGameConfigData::Get();
    if (!GameConfig || GameConfig->LoadingLevel.ToSoftObjectPath().IsNull())
    {
        UE_LOG(LogTemp, Error, TEXT("LoadingLevel not set in GameConfigData."));
        return;
    }

    FString LoadingMapPath = GameConfig->LoadingLevel.ToSoftObjectPath().GetLongPackageName();

    // 2. 로딩 레벨로 전환
    UGameplayStatics::OpenLevel(GetWorld(), FName(*LoadingMapPath));
}

void ULoadingSubsystem::StartLoadingAssets()
{
    ActiveHandles.Empty();

    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

    // 4. 리소스 목록 로드
    for (const auto& AssetPath : PendingResourcesPath)
    {
        if (!AssetPath.IsNull())
        {
            auto Handle = Streamable.RequestAsyncLoad(
                AssetPath,
                FStreamableDelegate(),
                FStreamableManager::AsyncLoadHighPriority
            );
            ActiveHandles.Add(AssetPath , Handle);
        }
    }

    auto LevelSoftPath = PendingTargetLevel.ToSoftObjectPath();
    // 5. 타겟 레벨 로드 (비동기, 하지만 Visible 상태로는 안 띄움)
    auto LevelHandle = Streamable.RequestAsyncLoad(
        LevelSoftPath,
        FStreamableDelegate::CreateUObject(this, &ULoadingSubsystem::OpenTargetLevel),
        FStreamableManager::AsyncLoadHighPriority
    );
    ActiveHandles.Add(LevelSoftPath, LevelHandle);
}

void ULoadingSubsystem::OpenTargetLevel() const
{
    FString TargetMapPath = PendingTargetLevel.ToSoftObjectPath().GetLongPackageName();
    UGameplayStatics::OpenLevel(GetWorld(), FName(*TargetMapPath));
}

TArray<FSoftObjectPath> ULoadingSubsystem::GetAllLoadedAssets() const
{
    return LoadedAssets.Array();
}

void ULoadingSubsystem::Deinitialize()
{
    Super::Deinitialize();
    auto Array = GetAllLoadedAssets();
    for (const auto& Path : Array)
    {
        RequestUnload(Path);
    }
}

ULoadingSubsystem::ULoadingSubsystem()
{
    
}
