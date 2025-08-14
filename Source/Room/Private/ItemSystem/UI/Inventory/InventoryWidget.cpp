// Fill out your copyright notice in the Description page of Project Settings.


#include "Room/Public/ItemSystem/UI/InventoryWidget/InventoryWidget.h"

#include "Components/UniformGridPanel.h"
#include "GameFramework/PlayerState.h"
#include "ItemSystem/InventoryComponent/InventoryComponent.h"
#include "ItemSystem/UI/InventorySlotWidget/InventorySlotWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/UISubsystem.h"


void UInventoryWidget::UpdateInventorySlotByIndex(int32 Index, const FInventorySlot& SlotInfo)
{
	if (InventorySlotWidgets.IsValidIndex(Index))
	{
		InventorySlotWidgets[Index]->UpdateSlotWidget(SlotInfo);
	}

}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InventoryComponent = UGameplayStatics::GetPlayerState(this, 0)->FindComponentByClass<UInventoryComponent>();

	int32 TargetCount = InventoryComponent->GetInventorySize();
	for (int32 i = 0; i < TargetCount; i++)
	{
		auto SlotWidget = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
		SlotWidget->SlotIndex = i;
		InventorySlotWidgets.Add(SlotWidget);
		UniformGridPanel_Slots->AddChildToUniformGrid(SlotWidget, i / 5, i % 5);
	}

}

FReply UInventoryWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::I)
	{
		if (UUISubsystem* Sub = GetGameInstance()->GetSubsystem<UUISubsystem>())
		{
			Sub->ToggleInventory();
			return FReply::Handled();
		}
	}
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}