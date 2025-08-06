#include "UI/Widget/HUDWidget.h"
#include "UI/HUD/ObjectiveWidget.h"
#include "UI/HUD/WeaponInfoWidget.h"
#include "UI/HUD/CrosshairWidget.h"
#include "UI/HUD/HealthWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HUDWidget)

void UHUDWidget::UpdateWeaponInfo(int32 CurrentAmmo, int32 TotalAmmo)
{
    if (WeaponInfoWidget)
    {
        WeaponInfoWidget->UpdateAmmo(CurrentAmmo, TotalAmmo);
    }
}

void UHUDWidget::UpdateObjective(int32 RangedKills, int32 RangedTotal, int32 MeleeKills, int32 MeleeTotal)
{
    if (ObjectiveWidget)
    {
        ObjectiveWidget->UpdateObjective(RangedKills, RangedTotal, MeleeKills, MeleeTotal);
    }
}

void UHUDWidget::UpdateHealth(float HealthRatio)
{
    if (HealthWidget)
    {
        HealthWidget->UpdateHealth(HealthRatio);
    }
}