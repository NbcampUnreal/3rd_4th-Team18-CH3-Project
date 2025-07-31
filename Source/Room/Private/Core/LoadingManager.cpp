#include "Core/LoadingManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LoadingManager)

void ULoadingManager::RequestLoad(const TSoftObjectPtr<UObject>& Asset, FOnAssetLoadComplete OnComplete)
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

void ULoadingManager::RequestLoadBatch(const TArray<TSoftObjectPtr<UObject>>& Assets, FOnAssetLoadComplete OnComplete)
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

void ULoadingManager::RequestUnload(const TSoftObjectPtr<UObject>& Asset)
{
    FSoftObjectPath Path = Asset.ToSoftObjectPath();

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

bool ULoadingManager::IsLoaded(const TSoftObjectPtr<UObject>& Asset) const
{
    return LoadedAssets.Contains(Asset.ToSoftObjectPath());
}

void ULoadingManager::UnloadUnusedAssets(const TSet<FSoftObjectPath>& KeepList)
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

void ULoadingManager::GetOrLoadAsset(const TSoftObjectPtr<UObject>& Asset, FOnAssetReady OnReady)
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