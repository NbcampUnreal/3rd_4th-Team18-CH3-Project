#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

UCLASS()
class ROOM_API UPauseMenuWidget : public UUserWidget {

	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ResumeGame();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ReturnToMainMenu();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ExitGameFromPause();
};