#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"

#include "RoomData.generated.h"

USTRUCT(BlueprintType)
struct FPoolableActorData 
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pool")
    TSoftClassPtr<AActor> ActorClass = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pool")
    int32 InitialPoolSize = 0;
};


USTRUCT(BlueprintType)
struct FRoomData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UWorld> Level;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Preload")
    TArray<FPoolableActorData> PoolAbleActorInfos;
    // 기타 로딩용 데이터들 파티클, 사운드, 동적 젠 되는 몬스터 등.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preload")
	TArray<TSoftObjectPtr<UObject>> PreloadAssets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Preload|Monsters")
	TArray<FDataTableRowHandle>	Monsters; 
	// UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Preload|Item")
	// TArray<FDataTableRowHandle>	Items;
};
