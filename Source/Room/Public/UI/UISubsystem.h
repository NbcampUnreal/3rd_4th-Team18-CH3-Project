#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystem/RoomSubsystem.h"
#include "Engine/DataTable.h"

#include "UISubsystem.generated.h"

class UMainMenuWidget;
class UPauseMenuWidget;
class UHUDWidget;
class ULoadingScreenWidget;
class UInventoryWidget;

UCLASS()
class ROOM_API UUISubsystem : public URoomSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void InitUIResources();
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
	void UpdateWeaponInfo(UTexture2D* Icon, FName Name, int32 AmmoCount);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateObjective(int32 RangedKill, int32 RangedTotal, int32 MeleeKill, int32 MeleeTotal);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHealth(float HealthRatio);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowLoadingScreen();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideLoadingScreen();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowHitMarkerOnHUD();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowKillMarkerOnHUD();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleInventory();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowDamageNumber(int32 Damage, FVector WorldLocation);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowGuideMessage(const FText& Message);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowInteractMessage(const FText& Message);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideInteractMessage();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateScore(int32 NewScore);

	UFUNCTION(BlueprintPure)
	bool IsInventoryOpen() const;


private:
	TWeakObjectPtr<UMainMenuWidget> MainMenuWidget;
	TWeakObjectPtr<UPauseMenuWidget> PauseMenuWidget;
	TWeakObjectPtr<UHUDWidget> HUDWidget;
	TWeakObjectPtr<ULoadingScreenWidget> LoadingWidget;
	TWeakObjectPtr<UInventoryWidget> InventoryWidget;

	UPROPERTY()
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY()
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY()
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	UPROPERTY()
	TSubclassOf<ULoadingScreenWidget> LoadingWidgetClass;

	UPROPERTY()
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	TSubclassOf<class ADamageTextActor> DamageTextActorClass;

	UPROPERTY(Transient)
	UUserWidget* MainMenuInstance = nullptr;

	UPROPERTY(Transient)
	UUserWidget* PauseMenuInstance = nullptr;

	UPROPERTY(Transient)
	UUserWidget* HUDInstance = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI Data")
	int32 UIDataID = 1;
};