#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystem/RoomSubsystem.h"

#include "UISubsystem.generated.h"

class UMainMenuWidget;
class UPauseMenuWidget;
class UHUDWidget;

UCLASS()
class ROOM_API UUISubsystem : public URoomSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetUIInputMode();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetGameInputMode();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowMainMenu();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowPauseMenu();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HidePauseMenu();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowHUD();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideHUD();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateAmmo(int32 Current, int32 Total);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateObjective(int32 RangedKill, int32 RangedTotal, int32 MeleeKill, int32 MeleeTotal);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHealth(float HealthRatio);

private:
	TWeakObjectPtr<UMainMenuWidget> MainMenuWidget;
	TWeakObjectPtr<UPauseMenuWidget> PauseMenuWidget;
	TWeakObjectPtr<UHUDWidget> HUDWidget;

	UPROPERTY()
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY()
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY()
	TSubclassOf<UHUDWidget> HUDWidgetClass;
};