#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/StreamableManager.h"
#include "UObject/SoftObjectPtr.h"
#include "LoadingManager.generated.h"

DECLARE_DELEGATE(FOnAssetLoadComplete)

UCLASS()
class ROOM_API ULoadingManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    void RequestLoad(const TSoftObjectPtr<UObject>& Asset, FOnAssetLoadComplete OnComplete);
    void RequestLoadBatch(const TArray<TSoftObjectPtr<UObject>>& Assets, FOnAssetLoadComplete OnComplete);

    void RequestUnload(const TSoftObjectPtr<UObject>& Asset);
    bool IsLoaded(const TSoftObjectPtr<UObject>& Asset) const; 

private:
    FStreamableManager StreamableManager;

    // 현재 로딩 중인 핸들들 추적
    TMap<FSoftObjectPath, TSharedPtr<FStreamableHandle>> ActiveHandles;

    // 이미 로드 완료된 오브젝트 목록
    TSet<FSoftObjectPath> LoadedAssets;
};

