#include "UI/HUD/ObjectiveWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ObjectiveWidget)

void UObjectiveWidget::UpdateObjective(
	int32 RangedKills,
	int32 RangedTotal,
	int32 MeleeKills,
	int32 MeleeTotal
)
{
	OnRangedObjectiveUpdated(RangedKills, RangedTotal);

	OnMeleeObjectiveUpdated(MeleeKills, MeleeTotal);

	const bool bClear = (RangedKills >= RangedTotal) && (MeleeKills >= MeleeTotal);

	OnStatusUpdated(bClear);
}