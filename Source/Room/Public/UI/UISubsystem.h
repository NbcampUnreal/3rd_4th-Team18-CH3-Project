#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystem/RoomSubsystem.h"

#include "Widget/MainMenuWidget.h"
#include "Widget/PauseMenuWidget.h"
#include "Widget/HUDWidget.h"

#include "UISubsystem.generated.h"

class UMainmenuWidget;

UCLASS()
class ROOM_API UUISubsystem : public URoomSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// UI Management
	void ShowMainMenu();
	void ShowPauseMenu();
	void HidePauseMenu();
	void ShowHUD();
	void HideHUD();

	void UpdateAmmo(int32 Current, int32 Total);
	void UpdateObjective(
		int32 RangedKill,
		int32 RangedTotal,
		int32 MeleeKill,
		int32 MeleeTotal
	);
	void UpdateHealth(float HealthRaito);

private:
	TWeakObjectPtr<UMainMenuWidget> MainMenuWidget;
	TWeakObjectPtr<UPauseMenuWidget> PauseMenuWidget;
	TWeakObjectPtr<UHUDWidget> HUDWidget;

	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	void InitializeWidgetClasses();
};
