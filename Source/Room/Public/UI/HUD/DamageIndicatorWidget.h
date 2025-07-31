#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageIndicatorWidget.generated.h"

UCLASS()
class ROOM_API UDamageIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowDamageFromDirection(float FromAngle);
};
