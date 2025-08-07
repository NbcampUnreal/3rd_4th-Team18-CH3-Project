#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class ROOM_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ExitGame();
};