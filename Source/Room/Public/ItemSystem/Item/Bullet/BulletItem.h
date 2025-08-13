
#pragma once

#include "CoreMinimal.h"
#include "ItemSystem/Item/ItemBase/ItemBase.h"
#include "ItemSystem/Structure/BulletItemInfo.h"
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
	
	float GetInitialSpeed() const;
	float GetMaxSpeed() const;
	float GetAutoDestroyTime() const;
	int32 GetBulletID() const;	
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Item")
	FBulletItemInfo BulletItemInfo;
private:
	UFUNCTION(BlueprintCallable)
	void SetBulletItemInfo(const FBulletItemInfo& BulletInfo);
};
