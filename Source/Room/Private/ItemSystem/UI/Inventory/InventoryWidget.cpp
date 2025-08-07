// Fill out your copyright notice in the Description page of Project Settings.


#include "Room/Public/ItemSystem/UI/InventoryWidget/InventoryWidget.h"

#include "ItemSystem/UI/InventorySlotWidget/InventorySlotWidget.h"

void UInventoryWidget::UpdateInventorySlotByIndex(int32 Index, const FInventorySlot& SlotInfo)
{
	SlotWidgets[Index]->UpdateSlotWidget(SlotInfo);
}

