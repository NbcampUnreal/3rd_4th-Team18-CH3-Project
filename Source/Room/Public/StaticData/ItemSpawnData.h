#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"
#include "ItemSpawnData.generated.h"

USTRUCT(BlueprintType)
struct FItemSpawnData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    int32 ItemID = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    float SpawnProbability = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    float SpawnTime = 0.0f;
};
