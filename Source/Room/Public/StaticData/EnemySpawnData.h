#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"
#include "EnemySpawnData.generated.h"

USTRUCT(BlueprintType)
struct FEnemySpawnData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    int32 EnemyID = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    int32 Count = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    float SpawnTime = 0.0f;
};
