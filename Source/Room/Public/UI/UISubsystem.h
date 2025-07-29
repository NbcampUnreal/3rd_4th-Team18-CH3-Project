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

private:
	

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// HUD display controls
	void ShowHUD();
	void HideCurrentUI();

	// Real-time gameplay updates
	void UpdateHealth(float Current, float Max);
	void UpdateAmmo(int32 Current, int32 Max);
	void UpdateScore(int32 Score);
	void UpdateMission(const FString& Name, float Progress);
	void AddKillLog(const FString& Killer, const FString& Victim);
	void ShowHitMarker();
	void ShowDamageIndicator(float FromAngle);
	void ShowKillConfirm();

	// Menu & UI panels
	void ShowMainMenu();
	void ShowPauseMenu();
	void ShowInventory();

protected:
	
	TObjectPtr<UUserWidget> ActiveWidget;

	
	UPROPERTY(EditAnywhere, Category = "UI|HUD")
	TSoftClassPtr<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI|Menus")
	TSoftClassPtr<UUserWidget> MainMenuClass;

	UPROPERTY(EditAnywhere, Category = "UI|Menus")
	TSoftClassPtr<UUserWidget> PauseMenuClass;

	UPROPERTY(EditAnywhere, Category = "UI|Menus")
	TSoftClassPtr<UUserWidget> InventoryClass;

	UPROPERTY(EditAnywhere, Category = "UI|HUD")
	TSoftClassPtr<UUserWidget> HitMarkerClass;

	UPROPERTY(EditAnywhere, Category = "UI|HUD")
	TSoftClassPtr<UUserWidget> DamageIndicatorClass;

	UPROPERTY(EditAnywhere, Category = "UI|HUD")
	TSoftClassPtr<UUserWidget> KillConfirmClass;

	/** Instantiated widget instances */
	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> HUDInstance;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> MainMenuInstance;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> PauseMenuInstance;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> InventoryInstance;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> HitMarkerInstance;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> DamageIndicatorInstance;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> KillConfirmInstance;

};