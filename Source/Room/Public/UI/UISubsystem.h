#pragma once

#include "CoreMinimal.h"
#include "Subsystem/RoomSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UISubsystem.generated.h"

class UUserWidget;
class UHUDWidget;
class UMainMenuWidget;
class UPauseMenuWidget;
class UInventoryWidget;
class UHitMarkerWidget;
class UDamageIndicatorWidget;
class UKillConfirmWidget;

UCLASS()
class ROOM_API UUISubsystem : public URoomSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// UI Show Methods
	void ShowHUD();
	void ShowMainMenu();
	void ShowPauseMenu();
	void ShowInventory();
	void HideCurrentUI();

	// Game Info Update
	void UpdateHealth(float Current, float Max);
	void UpdateAmmo(int32 Current, int32 Max);
	void UpdateScore(int32 Score);
	void UpdateMission(const FString& Name, float Progress);
	void AddKillLog(const FString& Killer, const FString& Victim);

	// Feedback
	void ShowHitMarker();
	void ShowDamageIndicator(float FromAngle);
	void ShowKillConfirm();

protected:
	UPROPERTY(EditAnywhere, Category = "UI|Classes")
	TSoftClassPtr<UHUDWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI|Classes")
	TSoftClassPtr<UMainMenuWidget> MainMenuClass;

	UPROPERTY(EditAnywhere, Category = "UI|Classes")
	TSoftClassPtr<UPauseMenuWidget> PauseMenuClass;

	UPROPERTY(EditAnywhere, Category = "UI|Classes")
	TSoftClassPtr<UInventoryWidget> InventoryClass;

	UPROPERTY(EditAnywhere, Category = "UI|Classes")
	TSoftClassPtr<UHitMarkerWidget> HitMarkerClass;

	UPROPERTY(EditAnywhere, Category = "UI|Classes")
	TSoftClassPtr<UDamageIndicatorWidget> DamageIndicatorClass;

	UPROPERTY(EditAnywhere, Category = "UI|Classes")
	TSoftClassPtr<UKillConfirmWidget> KillConfirmClass;

	UPROPERTY(Transient) TObjectPtr<UHUDWidget> HUDWidget;
	UPROPERTY(Transient) TObjectPtr<UMainMenuWidget> MainMenuWidget;
	UPROPERTY(Transient) TObjectPtr<UPauseMenuWidget> PauseMenuWidget;
	UPROPERTY(Transient) TObjectPtr<UInventoryWidget> InventoryWidget;
	UPROPERTY(Transient) TObjectPtr<UHitMarkerWidget> HitMarkerWidget;
	UPROPERTY(Transient) TObjectPtr<UDamageIndicatorWidget> DamageIndicatorWidget;
	UPROPERTY(Transient) TObjectPtr<UKillConfirmWidget> KillConfirmWidget;

	UPROPERTY(Transient) TObjectPtr<UUserWidget> ActiveWidget;

	void ShowWidget(TSoftClassPtr<UUserWidget> WidgetClass, TObjectPtr<UUserWidget>& OutInstance);
};
