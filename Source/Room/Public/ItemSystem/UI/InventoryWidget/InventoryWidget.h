// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;
class UUniformGridPanel;
struct FInventorySlot;
class UInventorySlotWidget;

UCLASS()
class ROOM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateInventorySlotByIndex(int32 Index, const FInventorySlot& SlotInfo);
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUniformGridPanel> UniformGridPanel_Slots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UInventorySlotWidget> InventorySlotWidgetClass;

	UPROPERTY()
	TArray<TObjectPtr<UInventorySlotWidget>> InventorySlotWidgets;
	
	TWeakObjectPtr<UInventoryComponent> InventoryComponent;
};
