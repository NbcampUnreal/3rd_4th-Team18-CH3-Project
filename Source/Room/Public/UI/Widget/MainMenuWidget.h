#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class ROOM_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowMainOptions();

};
