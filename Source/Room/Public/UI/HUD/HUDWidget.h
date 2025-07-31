#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

UCLASS()
class ROOM_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Functions for HUD elements.. add here later
	void UpdateHealthBar(float Current, float Max);

};
