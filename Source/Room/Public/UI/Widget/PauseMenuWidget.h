#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

UCLASS()
class ROOM_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowPauseOptions();

};
