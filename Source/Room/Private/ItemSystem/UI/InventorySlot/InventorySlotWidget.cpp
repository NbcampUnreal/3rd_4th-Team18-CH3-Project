
#include "Room/Public/ItemSystem/UI/InventorySlotWidget/InventorySlotWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFramework/PlayerState.h"
#include "ItemSystem/InventoryComponent/InventoryComponent.h"
#include "ItemSystem/Item/ItemBase/ItemBase.h"
#include "ItemSystem/Structure/InventorySlot.h"
#include "Kismet/GameplayStatics.h"



void UInventorySlotWidget::UpdateSlotWidget(const FInventorySlot& SlotInfo)
{
	if (SlotInfo.Item.Get())
	{
		Button_Slot->SetIsEnabled(true);
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
		Button_Slot->SetIsEnabled(false);
		Image_Item->SetVisibility(ESlateVisibility::Collapsed);
		Text_ItemName->SetVisibility(ESlateVisibility::Collapsed);
		Text_Quantity->SetVisibility(ESlateVisibility::Collapsed);
		OnCancelClicked();
	}
}

void UInventorySlotWidget::UpdateSlotWidgetUsingInventoryInfo()
{
	UpdateSlotWidget(InventoryComponent->GetInventorySlotByIndex(SlotIndex));
}

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Text_Quantity->SetVisibility(ESlateVisibility::Collapsed);
	Image_Item->SetVisibility(ESlateVisibility::Collapsed);
	Text_ItemName->SetVisibility(ESlateVisibility::Collapsed);

	Button_Slot->OnClicked.AddDynamic(this,&ThisClass::OnSlotClicked);
	Button_ItemUse->OnClicked.AddDynamic(this,&ThisClass::OnItemUseClicked);
	Button_ItemDrop->OnClicked.AddDynamic(this,&ThisClass::OnItemDropClicked);
	Button_Cancel->OnClicked.AddDynamic(this,&ThisClass::OnCancelClicked);

	Button_Decrease->OnClicked.AddDynamic(this,&ThisClass::OnDecreaseQuantityClicked);
	Button_Increase->OnClicked.AddDynamic(this,&ThisClass::OnIncreaseQuantityClicked);
	Button_DropQuantityCheck->OnClicked.AddDynamic(this,&ThisClass::OnDropQuantityCheckClicked);
	
	InventoryComponent = UGameplayStatics::GetPlayerState(this,0)->FindComponentByClass<UInventoryComponent>();
	if (InventoryComponent.IsValid())
	{
		InventoryComponent->OnSlotChanged.AddDynamic(this,&ThisClass::UpdateSlotWidgetUsingInventoryInfo);
	}
	UpdateSlotWidgetUsingInventoryInfo();
}

void UInventorySlotWidget::OnSlotClicked()
{
	VB_ActionMenu->SetVisibility(ESlateVisibility::Visible);
}

void UInventorySlotWidget::OnItemUseClicked()
{
	InventoryComponent->UseItem(SlotIndex);
	VB_ActionMenu->SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlotWidget::OnItemDropClicked()
{
	Border_SetDropQuantity->SetVisibility(ESlateVisibility::Visible);
	Text_DropQuantity->SetText(FText::AsNumber(DropQuantity));
}

void UInventorySlotWidget::OnCancelClicked()
{
	VB_ActionMenu->SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlotWidget::OnDecreaseQuantityClicked()
{
	DropQuantity--;
	if (DropQuantity < 0)
	{
		DropQuantity = 0;
	}

	Text_DropQuantity->SetText(FText::AsNumber(DropQuantity));
}

void UInventorySlotWidget::OnIncreaseQuantityClicked()
{
	DropQuantity++;
	if (DropQuantity>InventoryComponent->GetInventorySlotByIndex(SlotIndex).Quantity)
	{
		DropQuantity = InventoryComponent->GetInventorySlotByIndex(SlotIndex).Quantity;
	}
	Text_DropQuantity->SetText(FText::AsNumber(DropQuantity));
}

void UInventorySlotWidget::OnDropQuantityCheckClicked()
{
	Border_SetDropQuantity->SetVisibility(ESlateVisibility::Hidden);
	VB_ActionMenu->SetVisibility(ESlateVisibility::Hidden);
	InventoryComponent->DropItemFromInventory(SlotIndex,DropQuantity);
	DropQuantity = 1;
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
