
#pragma once

#include "CoreMinimal.h"
#include "ItemSystem/Item/ItemBase/ItemBase.h"
#include "StaticData/BulletItemData.h"
#include "BulletItem.generated.h"

UCLASS(Blueprintable)
class ROOM_API UBulletItem : public UItemBase
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
	virtual bool IsUsable_Implementation() const override;
	
	int32 GetBulletID() const;
	float GetBulletInitialSpeed() const;
	float GetBulletMaxSpeed() const;
	float GetBulletAutoDestroyTime() const;

	UFUNCTION(BlueprintCallable)
	void SetBulletItemInfo(const FBulletItemData& _BulletItemInfo);

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Item")
	FBulletItemData BulletItemInfo;

};
