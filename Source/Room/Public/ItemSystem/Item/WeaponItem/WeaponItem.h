
#pragma once

#include "CoreMinimal.h"
#include "ItemSystem/Item/ItemBase/ItemBase.h"
#include "StaticData/WeaponItemData.h"
#include "WeaponItem.generated.h"

struct FWeaponItemData;

UCLASS(Blueprintable)
class ROOM_API UWeaponItem : public UItemBase
{
	GENERATED_BODY()
public:
	virtual void Use_Implementation(AActor* User) override;
	virtual FName GetItemID_Implementation() const override;
	virtual FName GetItemName_Implementation() const override;
	virtual FText GetItemDescription_Implementation() const override;
	virtual UTexture2D* GetItemIcon_Implementation() const override;
	virtual UStaticMesh* GetItemMesh_Implementation() const override;
	virtual bool IsConsumable_Implementation() const override;
	virtual int32 GetMaxStackCount_Implementation() const override;
	float GetWeaponAttackDamage() const;
	float GetWeaponAttackSpeed() const;
	float GetWeaponAttackRecoil() const;
	float GetWeaponAttackRange() const;
	int32 GetWeaponBulletID() const;
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Item")
	FWeaponItemData WeaponItemInfo;
private:
	UFUNCTION(BlueprintCallable)
	void SetWeaponItemInfo(const FWeaponItemData& WeaponInfo);
};
