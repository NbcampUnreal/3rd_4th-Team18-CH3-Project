#include "UI/Widget/HUDWidget.h"
#include "UI/HUD/ObjectiveWidget.h"
#include "UI/HUD/WeaponInfoWidget.h"
#include "UI/HUD/CrosshairWidget.h"
#include "UI/HUD/HealthWidget.h"
#include "UI/HUD/GuideMessageWidget.h"
#include "UI/HUD/InteractMessageWidget.h"
#include "ItemSystem/UI/InventoryWidget/InventoryWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HUDWidget)

void UHUDWidget::UpdateWeaponAndAmmo(UTexture2D* Icon, FName Name, int32 AmmoCount)
{
    if (WeaponInfoWidget)
    {
        WeaponInfoWidget->SetWeaponDisplay(Icon, Name, AmmoCount);
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

void UHUDWidget::ShowCrosshairHitMarker()
{
    if (CrosshairWidget)
    {
        CrosshairWidget->ShowHitMarker();
    }
}

void UHUDWidget::ShowKillMarker()
{
    if (CrosshairWidget)
    {
        CrosshairWidget->ShowKillMarker();
    }
}

void UHUDWidget::ShowGuideMessage(const FText& Message)
{
    if (GuideMessageWidget)
    {
        GuideMessageWidget->ShowGuideMessage(Message);
    }
}

void UHUDWidget::ShowInteractMessage(const FText& Message)
{
    if (InteractMessageWidget)
    {
        InteractMessageWidget->ShowMessage(Message);
    }
}

void UHUDWidget::HideInteractMessage()
{
    if (InteractMessageWidget)
    {
        InteractMessageWidget->HideMessage();
    }
}

void UHUDWidget::SetInventoryVisible(bool bVisible)
{
    if (!InventoryWidget)
    {
        return;
    }

    InventoryWidget->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

    if (bVisible)
    {
        InventoryWidget->SetUserFocus(GetOwningPlayer());
    }
}

bool UHUDWidget::IsInventoryVisible() const
{
    if (!InventoryWidget)
    {
        return false;
    }

    return InventoryWidget->GetVisibility() == ESlateVisibility::Visible;
}