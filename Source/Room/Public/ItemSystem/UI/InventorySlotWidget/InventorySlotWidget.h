// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UTextBlock;
class UImage;
struct FInventorySlot;
/**
 * 
 */
UCLASS()
class ROOM_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateSlotWidget(const FInventorySlot& SlotInfo);
protected:
	virtual void NativeConstruct() override;
	
private:
	void UpdateQuantityText(const FInventorySlot& SlotInfo);
	void UpdateItemIcon(const FInventorySlot& SlotInfo);
	void UpdateItemName(const FInventorySlot& SlotInfo);
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Item;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Quantity;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_ItemName;
};
