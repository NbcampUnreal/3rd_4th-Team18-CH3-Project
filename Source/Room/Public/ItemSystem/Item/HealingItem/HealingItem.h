// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSystem/Item/ItemBase/ItemBase.h"
#include "StaticData/HealingItemData.h"
#include "HealingItem.generated.h"

UCLASS(Blueprintable)
class ROOM_API UHealingItem : public UItemBase
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
	float GetHealingAmount() const;
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Item")
	FHealingItemData HealingItemInfo;
private:
	UFUNCTION(BlueprintCallable)
	void SetHealingItemInfo(const FHealingItemData& HealingInfo);
};
