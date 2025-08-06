#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class ROOM_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    // Widget Bindings
    UPROPERTY(meta = (BindWidget)) UButton* GameStartButton;
    UPROPERTY(meta = (BindWidget)) UButton* ExitButton;
    UPROPERTY(meta = (BindWidget)) UTextBlock* GameStartText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* ExitText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* DescriptionText;

	// Button Click Events
    UFUNCTION() void OnGameStartClicked();
    UFUNCTION() void OnExitClicked();

	// Button Hover Events
    UFUNCTION() void OnGameStartHovered();
    UFUNCTION() void OnExitHovered();
    UFUNCTION() void OnButtonUnhovered();
};