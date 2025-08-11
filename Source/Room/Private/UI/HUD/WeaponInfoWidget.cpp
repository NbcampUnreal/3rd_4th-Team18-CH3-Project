#include "UI/HUD/WeaponInfoWidget.h"
#include "Components/TextBlock.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WeaponInfoWidget)

void UWeaponInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UWeaponInfoWidget::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo, int32 TotalAmmo)
{
	if (CurrentAmmoText)
	{
		CurrentAmmoText->SetText(FText::AsNumber(CurrentAmmo));
	}

	if (TotalAmmoText)
	{
		TotalAmmoText->SetText(FText::AsNumber(TotalAmmo));
	}

	OnAmmoUpdated(CurrentAmmo, MaxAmmo);
}