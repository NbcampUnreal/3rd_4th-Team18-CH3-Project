#pragma once

#include "CoreMinimal.h"
#include "StaticData/StaticData.h"
#include "HealingItemInfo.generated.h"

USTRUCT(BlueprintType)
struct FHealingItemInfo : public FStaticData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	FName ItemID;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	FName ItemName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	FText ItemDescription;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	TObjectPtr<UTexture2D> ItemIcon;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	TObjectPtr<UStaticMesh> ItemMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	bool bConsumable = true;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	int32 MaxStackCount = 9;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	float HealingAmount;
};
