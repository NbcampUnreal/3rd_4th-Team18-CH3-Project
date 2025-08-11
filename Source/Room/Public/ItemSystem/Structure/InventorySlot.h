#pragma once

#include "CoreMinimal.h"
#include "InventorySlot.generated.h"

class UItemBase;

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	TObjectPtr<UItemBase> Item = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	int32 Quantity = 0;
};
