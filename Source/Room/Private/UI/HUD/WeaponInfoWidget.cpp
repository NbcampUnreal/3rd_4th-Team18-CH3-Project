#include "UI/HUD/WeaponInfoWidget.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WeaponInfoWidget)

void UWeaponInfoWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CurrentAmmoText)
    {
        CurrentAmmoText->SetText(FText::FromString(TEXT("0")));
    }

    if (TotalAmmoText)
    {
        TotalAmmoText->SetText(FText::FromString(TEXT("0")));
    }

    if (WeaponSlotText)
    {
        WeaponSlotText->SetText(FText::FromString(TEXT("01")));
    }

    // ź �ð�ȭ �� �ʱ�ȭ
    if (AmmoBarContainer && AmmoUnitWidgetClass)
    {
        AmmoBarContainer->ClearChildren();
        AmmoImages.Empty();

        for (int32 i = 0; i < MaxAmmoUnits; ++i)
        {
            UImage* AmmoImage = NewObject<UImage>(this, UImage::StaticClass());
            if (AmmoImage)
            {
                AmmoBarContainer->AddChild(AmmoImage);
                AmmoImages.Add(AmmoImage);
            }
        }
    }
}

void UWeaponInfoWidget::UpdateAmmo(int32 CurrentAmmo, int32 TotalAmmo)
{
    if (CurrentAmmoText)
    {
        CurrentAmmoText->SetText(FText::AsNumber(CurrentAmmo));
    }

    if (TotalAmmoText)
    {
        TotalAmmoText->SetText(FText::AsNumber(TotalAmmo));
    }

    // ź �ð�ȭ �� ������Ʈ
    for (int32 i = 0; i < AmmoImages.Num(); ++i)
    {
        if (AmmoImages[i])
        {
            bool bActive = i < CurrentAmmo;
            AmmoImages[i]->SetOpacity(bActive ? 1.0f : 0.2f); // ������ ��Ȱ�� ǥ��
        }
    }
}