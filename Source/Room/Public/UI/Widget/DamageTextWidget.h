#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageTextWidget.generated.h"

UCLASS()
class ROOM_API UDamageTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "DamageText")
	void SetDamageText(int32 Damage);
};
