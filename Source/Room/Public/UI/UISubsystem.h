#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystem/RoomSubsystem.h"

#include "UISubsystem.generated.h"

class UMainMenuWidget;
class UPauseMenuWidget;
class UHUDWidget;

UCLASS(Config = Game, DefaultConfig)
class ROOM_API UUISubsystem : public URoomSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void SetUIInputMode();
	void SetGameInputMode();
	void ShowMainMenu();
	void ShowPauseMenu();
	void HidePauseMenu();
	void ShowHUD();
	void HideHUD();
	void UpdateAmmo(int32 Current, int32 Total);
	void UpdateObjective(int32 RangedKill, int32 RangedTotal, int32 MeleeKill, int32 MeleeTotal);
	void UpdateHealth(float HealthRatio);

private:
	TWeakObjectPtr<UMainMenuWidget> MainMenuWidget;
	TWeakObjectPtr<UPauseMenuWidget> PauseMenuWidget;
	TWeakObjectPtr<UHUDWidget> HUDWidget;

	UPROPERTY(Config)
	TSoftClassPtr<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(Config)
	TSoftClassPtr<UPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY(Config)
	TSoftClassPtr<UHUDWidget> HUDWidgetClass;

};