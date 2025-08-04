
#pragma once

#include "CoreMinimal.h"
#include "ItemSystem/Item/ItemBase/ItemBase.h"
#include "ItemSystem/Structure/WeaponItemInfo.h"
#include "WeaponItem.generated.h"

struct FWeaponItemInfo;

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
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Item")
	FWeaponItemInfo WeaponItemInfo;
private:
	UFUNCTION(BlueprintCallable)
	void SetWeaponItemInfo(const FWeaponItemInfo& WeaponInfo);
};
