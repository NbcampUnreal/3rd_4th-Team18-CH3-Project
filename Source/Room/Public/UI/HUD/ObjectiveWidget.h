#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveWidget.generated.h"

UCLASS()
class ROOM_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateObjective(
		int32 RangedKills,
		int32 RangedTotal,
		int32 MeleeKills,
		int32 MeleeTotal
	);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnRangedObjectiveUpdated(int32 Kills, int32 Total);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnMeleeObjectiveUpdated(int32 Kills, int32 Total);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnStatusUpdated(bool bIsCleared);
};