#pragma once

#include "CoreMinimal.h"
#include "StaticData/StaticData.h"
#include "BulletItemInfo.generated.h"

USTRUCT(BlueprintType)
struct FBulletItemInfo : public FStaticData
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
	int32 MaxStackCount = 999;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	bool bUsable = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	float BulletInitialSpeed = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	float BulletMaxSpeed = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	float BulletAutoDestroyTime = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	int32 BulletID;
};
