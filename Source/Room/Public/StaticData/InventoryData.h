#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"
#include "InventoryData.generated.h"

USTRUCT(BlueprintType)
struct FInventoryData : public FStaticData
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    TMap<int32, int32> ItemList;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    int32 MaxSlotSize = 0;
};
