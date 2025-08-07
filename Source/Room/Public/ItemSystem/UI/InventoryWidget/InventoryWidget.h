// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

struct FInventorySlot;
class UInventorySlotWidget;
/**
 * 
 */
UCLASS()
class ROOM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateInventorySlotByIndex(int32 Index, const FInventorySlot& SlotInfo);
private:
	UPROPERTY(meta=(BindWidget))
	TArray<TObjectPtr<UInventorySlotWidget>> SlotWidgets;
};
