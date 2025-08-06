#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"
#include "EnemySpawnData.h"
#include "ItemSpawnData.h"
#include "MapSpawnData.generated.h"

USTRUCT(BlueprintType)
struct FMapSpawnData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
    TArray<FEnemySpawnData> EnemySpawns;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
    TArray<FItemSpawnData> ItemSpawns;
};
