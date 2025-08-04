// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSystem/UI/HUD/RoomHUD.h"

#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "ItemSystem/UI/InventoryWidget/InventoryWidget.h"

URoomHUD::URoomHUD(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void URoomHUD::ToggleInventoryWidget()
{
	auto InventoryVisibility = InventoryWidget_RoomInventory->GetVisibility();
	switch (InventoryVisibility)
	{
		case ESlateVisibility::Collapsed:
			InventoryWidget_RoomInventory->SetVisibility(ESlateVisibility::Visible);
			break;
		default:
			InventoryWidget_RoomInventory->SetVisibility(ESlateVisibility::Collapsed);
			break;
	}
	
}

void URoomHUD::ShowInteractMessage(const FText& MSG)
{
	Overlay_PickupMessage->SetVisibility(ESlateVisibility::Visible);
	TEXT_PickupMessage->SetText(MSG);
}

void URoomHUD::HideInteractMessage()
{
	Overlay_PickupMessage->SetVisibility(ESlateVisibility::Collapsed);
}

void URoomHUD::NativeConstruct()
{
	Super::NativeConstruct();
	HideInteractMessage();
	InventoryWidget_RoomInventory->SetVisibility(ESlateVisibility::Collapsed);
}
