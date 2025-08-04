
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoomHUD.generated.h"

class UInventoryWidget;
class UTextBlock;
class UOverlay;

UCLASS()
class ROOM_API URoomHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	URoomHUD(const FObjectInitializer& ObjectInitializer);
	
	void ToggleInventoryWidget();
	void ShowInteractMessage(const FText& MSG);
	void HideInteractMessage();
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> Overlay_PickupMessage;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TEXT_PickupMessage;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UInventoryWidget> InventoryWidget_RoomInventory;
};
