// Fill out your copyright notice in the Description page of Project Settings.


#include "Room/Public/ItemSystem/UI/InventorySlotWidget/InventorySlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ItemSystem/Item/ItemBase/ItemBase.h"
#include "ItemSystem/Structure/InventorySlot.h"

void UInventorySlotWidget::UpdateSlotWidget(const FInventorySlot& SlotInfo)
{

	if (SlotInfo.Item.Get())
	{
		Image_Item->SetVisibility(ESlateVisibility::Visible);
		Text_ItemName->SetVisibility(ESlateVisibility::Visible);
		UpdateItemIcon(SlotInfo);
		UpdateItemName(SlotInfo);

		if (SlotInfo.Item->IsConsumable())
		{
			Text_Quantity->SetVisibility(ESlateVisibility::Visible);
			UpdateQuantityText(SlotInfo);
		}
		else
		{
			Text_Quantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		Image_Item->SetVisibility(ESlateVisibility::Collapsed);
		Text_ItemName->SetVisibility(ESlateVisibility::Collapsed);
		Text_Quantity->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Text_Quantity->SetVisibility(ESlateVisibility::Collapsed);
	Image_Item->SetVisibility(ESlateVisibility::Collapsed);
	Text_ItemName->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventorySlotWidget::UpdateQuantityText(const FInventorySlot& SlotInfo)
{
	FText QuantityText = FText::AsNumber(SlotInfo.Quantity);
	Text_Quantity->SetText(QuantityText);
}

void UInventorySlotWidget::UpdateItemIcon(const FInventorySlot& SlotInfo)
{
	Image_Item->SetBrushFromTexture(SlotInfo.Item->GetItemIcon());
}

void UInventorySlotWidget::UpdateItemName(const FInventorySlot& SlotInfo)
{
	FText ItemNameText = FText::FromName(SlotInfo.Item->GetItemName());
	Text_ItemName->SetText(ItemNameText);
}
