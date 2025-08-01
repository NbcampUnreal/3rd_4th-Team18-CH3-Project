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
	
	// Buttons
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GamePlayButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;

	// Button texts
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GamePlayText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ExitText;

	// Description texts
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DescriptionText;

private:
	// Callback.
	UFUNCTION()
	void OnGamePlayClicked();

	UFUNCTION()
	void OnExitClicked();

	UFUNCTION()
	void OnGamePlayHovered();

	UFUNCTION()
	void OnExitHovered();

	void ResetButtonColors();
};
