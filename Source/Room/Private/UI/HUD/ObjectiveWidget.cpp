#include "UI/HUD/ObjectiveWidget.h"
#include "Components/TextBlock.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ObjectiveWidget)

void UObjectiveWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StatusText)
	{
		StatusText->SetText(FText::GetEmpty());
	}
}

void UObjectiveWidget::UpdateObjective(int32 RangedKills, int32 RangedTotal, int32 MeleeKills, int32 MeleeTotal)
{
    if (RangedText)
    {
        RangedText->SetText(FText::Format(
            FText::FromString(TEXT("Ranged Minion {0}/{1}")),
            FText::AsNumber(RangedKills),
            FText::AsNumber(RangedTotal)
        ));
    }

    if (MeleeText)
    {
        MeleeText->SetText(FText::Format(
            FText::FromString(TEXT("Melee Minion {0}/{1}")),
            FText::AsNumber(MeleeKills),
            FText::AsNumber(MeleeTotal)
        ));
    }

    if (StatusText)
    {
        const bool bClear = (RangedKills >= RangedTotal) && (MeleeKills >= MeleeTotal);
        StatusText->SetText(bClear ? FText::FromString(TEXT("Objective Clear!")) : FText::GetEmpty());
    }
}