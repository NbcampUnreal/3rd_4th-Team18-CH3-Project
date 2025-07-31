#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillConfirmWidget.generated.h"

UCLASS()
class ROOM_API UKillConfirmWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowKillConfirm();
};
