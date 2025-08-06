#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponInfoWidget.generated.h"

class UTextBlock;
class UHorizontalBox;
class UImage;

UCLASS()
class ROOM_API UWeaponInfoWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget)) UTextBlock* CurrentAmmoText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* TotalAmmoText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* WeaponSlotText;

    UPROPERTY(meta = (BindWidget)) UHorizontalBox* AmmoBarContainer;

    // 탄 시각화용 이미지 프리팹 (UMG에만 존재)
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> AmmoUnitWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Ammo")
    int32 MaxAmmoUnits = 10;

    TArray<UImage*> AmmoImages;

public:
    void UpdateAmmo(int32 CurrentAmmo, int32 TotalAmmo);
};
