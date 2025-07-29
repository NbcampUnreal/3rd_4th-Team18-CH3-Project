#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UISubsystem.generated.h"

class UUserWidget;
class UHUDWidget;

UCLASS()
class ROOM_API UUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// UI display functions
	void ShowHUD();
	void ShowPauseMenu();
	void ShowResultScreen();
	void ShowInventory(); // For full-screen inventory that requires detailed view
	void HideCurrentUI();

	void UpdateHealth(float Current, float Max);
	void UpdateAmmo(int32 Current, int32 Max);
	void UpdateScore(int32 Score);

private:
	// Currently active widget displayed on screen
	TObjectPtr<UUserWidget> ActiveWidget;

	// UI Widget blueprint classes
	TSubclassOf<UHUDWidget> HUDClass;
	TSubclassOf<UUserWidget> PauseMenuClass;
	TSubclassOf<UUserWidget> ResultScreenClass;
	TSubclassOf<UUserWidget> InventoryClass;

	TObjectPtr<UHUDWidget> HUDWidget;

	void ShowWidget(TSubclassOf<UUserWidget> WidgetClass, TObjectPtr<UUserWidget>& OutWidget);
};
