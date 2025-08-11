// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UBorder;
class UInventoryComponent;
class UVerticalBox;
class UButton;
class UTextBlock;
class UImage;
struct FInventorySlot;
UCLASS()
class ROOM_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateSlotWidget(const FInventorySlot& SlotInfo);
	UFUNCTION(BlueprintCallable)
	void UpdateSlotWidgetUsingInventoryInfo();

	int32 SlotIndex;

	void HideActionMenu();
	void HideDropMenu();

protected:

	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void OnSlotClicked();
	UFUNCTION()
	void OnItemUseClicked();
	UFUNCTION()
	void OnItemDropClicked();
	UFUNCTION()
	void OnCancelClicked();
	
	UFUNCTION()
	void OnDecreaseQuantityClicked();
	UFUNCTION()
	void OnIncreaseQuantityClicked();
	UFUNCTION()
	void OnDropQuantityCheckClicked();
	
	void UpdateQuantityText(const FInventorySlot& SlotInfo);
	void UpdateItemIcon(const FInventorySlot& SlotInfo);
	void UpdateItemName(const FInventorySlot& SlotInfo);

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Slot;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Item;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Quantity;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_ItemName;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> VB_ActionMenu;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_ItemUse;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_ItemDrop;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Cancel;
	UPROPERTY(meta=(BindWidget))

	
	TObjectPtr<UBorder> Border_SetDropQuantity;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_DropQuantity;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Decrease;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Increase;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_DropQuantityCheck;

	int32 DropQuantity = 1;
	
	TWeakObjectPtr<UInventoryComponent> InventoryComponent;
};
