#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"
#include "StatData.generated.h"

USTRUCT(BlueprintType)
struct FStatData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
    float HP = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
    float Attack = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
    float Defense = 0.0f;
};
