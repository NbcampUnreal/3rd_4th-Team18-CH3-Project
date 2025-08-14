#include "UI/HUD/WeaponInfoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WeaponInfoWidget)

void UWeaponInfoWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ClearDisplay();
}

void UWeaponInfoWidget::SetWeaponDisplay(UTexture2D* Icon, FName Name, int32 AmmoCount)
{
    if (WeaponIcon != nullptr)
    {
        WeaponIcon->SetBrushFromTexture(Icon);
    }

    if (WeaponNameText != nullptr)
    {
        WeaponNameText->SetText(FText::FromName(Name));
    }

    if (AmmoCountText != nullptr)
    {
        AmmoCountText->SetText(FText::AsNumber(AmmoCount));
    }

    OnWeaponDisplayUpdated();
}

void UWeaponInfoWidget::ClearDisplay()
{
    if (WeaponIcon != nullptr)
    {
        WeaponIcon->SetBrushFromTexture(nullptr);
    }

    if (WeaponNameText != nullptr)
    {
        WeaponNameText->SetText(FText::GetEmpty());
    }

    if (AmmoCountText != nullptr)
    {
        AmmoCountText->SetText(FText::FromString(TEXT("0")));
    }
}
