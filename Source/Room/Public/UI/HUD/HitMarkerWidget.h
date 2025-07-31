#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitMarkerWidget.generated.h"

UCLASS()
class ROOM_API UHitMarkerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowHitMarker();

};
