#include "UI/HUD/DamageNumberWidget.h"
#include "Components/TextBlock.h"

void UDamageNumberWidget::SetDamage(int32 Value)
{
    if (DamageText)
    {
        DamageText->SetText(FText::AsNumber(Value));
    }
}