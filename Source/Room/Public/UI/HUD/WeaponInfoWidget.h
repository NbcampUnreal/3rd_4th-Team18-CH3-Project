#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

class UImage;
class UTextBlock;
class UTexture2D;

#include "WeaponInfoWidget.generated.h"

UCLASS()
class ROOM_API UWeaponInfoWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetWeaponDisplay(UTexture2D* Icon, FName Name, int32 AmmoCount);

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void ClearDisplay();

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidgetOptional))
    UImage* WeaponIcon;

    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* WeaponNameText;

    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* AmmoCountText;

    UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
    void OnWeaponDisplayUpdated();
};
