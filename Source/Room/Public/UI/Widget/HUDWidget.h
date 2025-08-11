#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UObjectiveWidget;
class UWeaponInfoWidget;
class UCrosshairWidget;
class UHealthWidget;

UCLASS()
class ROOM_API UHUDWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget)) UObjectiveWidget* ObjectiveWidget;
    UPROPERTY(meta = (BindWidget)) UWeaponInfoWidget* WeaponInfoWidget;
    UPROPERTY(meta = (BindWidget)) UCrosshairWidget* CrosshairWidget;
    UPROPERTY(meta = (BindWidget)) UHealthWidget* HealthWidget;

public:
    UFUNCTION(BlueprintCallable)
    void UpdateWeaponInfo(int32 CurrentAmmo,int32 MaxAmmo, int32 TotalAmmo);
    UFUNCTION(BlueprintCallable)
    void UpdateObjective(int32 RangedKills, int32 RangedTotal, int32 MeleeKills, int32 MeleeTotal);
    UFUNCTION(BlueprintCallable)
    void UpdateHealth(float HealthRatio);

protected:
    
};