#pragma once

#include "CoreMinimal.h"
#include "StaticData/StaticData.h"
#include "WeaponItemData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponItemData : public FStaticData
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
	bool bConsumable = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	int32 MaxStackCount = 1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	bool bUsable = true;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	float WeaponAttackDamage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	float WeaponAttackSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	float WeaponRecoil;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	float WeaponAttackRange;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	int32 WeaponBulletID = 0;
};
