#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"
#include "GameFramework/Actor.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSoftObjectPtr<UStaticMesh> Mesh = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSoftObjectPtr<UMaterialInterface> Material = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSoftClassPtr<AActor> EffectClass = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    int32 MaxStack = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    int32 ItemCount = 0;
};
