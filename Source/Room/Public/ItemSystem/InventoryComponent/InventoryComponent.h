
#pragma once

#include "CoreMinimal.h"
#include "ItemSystem/Structure/InventorySlot.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"
class AFieldItem;
class URoomHUD;
class UInventoryWidget;
class UItemBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlotChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable)
class ROOM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UInventoryComponent();
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 AddItemToInventory(UItemBase* NewItem, int32 Quantity);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(int32 SlotIndex);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool DropItemFromInventory(int32 SlotIndex,int32 Quantity);

	const FInventorySlot& GetInventorySlotByIndex(int32 SlotIndex) const;
	int32 GetInventorySize() const { return InventorySize;}

	FOnSlotChanged OnSlotChanged;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventorySlot> InventorySlots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 InventorySize = 20;
	
private:
	int32 AddStackableItem(UItemBase* NewItem, int32 Quantity);
	int32 AddNonStackableItem(UItemBase* NewItem);
	bool UseStackableItem(int32 SlotIndex);
	bool UseNonStackableItem(int32 SlotIndex);
	bool DropStackableItem(int32 SlotIndex,int32 Quantity);
	bool DropNonStackableItem(int32 SlotIndex);

};
