#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UObjectiveWidget;
class UWeaponInfoWidget;
class UCrosshairWidget;
class UHealthWidget;
class UGuideMessageWidget;
class UInteractMessageWidget;
class UInventoryWidget;

UCLASS()
class ROOM_API UHUDWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget)) UObjectiveWidget* ObjectiveWidget;
    UPROPERTY(meta = (BindWidget)) UWeaponInfoWidget* WeaponInfoWidget;
    UPROPERTY(meta = (BindWidget)) UCrosshairWidget* CrosshairWidget;
    UPROPERTY(meta = (BindWidget)) UHealthWidget* HealthWidget;
    UPROPERTY(meta = (BindWidget)) UGuideMessageWidget* GuideMessageWidget;
    UPROPERTY(meta = (BindWidget)) UInteractMessageWidget* InteractMessageWidget;

public:
    UFUNCTION(BlueprintCallable)
    void UpdateWeaponInfo(int32 CurrentAmmo,int32 MaxAmmo, int32 TotalAmmo);
    UFUNCTION(BlueprintCallable)
    void UpdateObjective(int32 RangedKills, int32 RangedTotal, int32 MeleeKills, int32 MeleeTotal);
    UFUNCTION(BlueprintCallable)
    void UpdateHealth(float HealthRatio);

    UFUNCTION(BlueprintCallable)
    void ShowCrosshairHitMarker();
    UFUNCTION(BlueprintCallable)
    void ShowKillMarker();

    UFUNCTION(BlueprintCallable)
    void ShowGuideMessage(const FText& Message);
    UFUNCTION(BlueprintCallable)
    void ShowInteractMessage(const FText& Message);
    UFUNCTION(BlueprintCallable)
    void HideInteractMessage();

    UFUNCTION(BlueprintCallable)
    void UpdateScore(int32 NewScore)
    {
        OnScoreUpdated(NewScore);
    }

    UFUNCTION(BlueprintImplementableEvent)
    void OnScoreUpdated(int32 NewScore);

    UFUNCTION(BlueprintCallable)
    void SetInventoryVisible(bool bVisible);

    UFUNCTION(BlueprintPure)
    bool IsInventoryVisible() const;

protected:
    
    UPROPERTY(meta=(BindWidgetOptional))
    TObjectPtr<UInventoryWidget> InventoryWidget;
};