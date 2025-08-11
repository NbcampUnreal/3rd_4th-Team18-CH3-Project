#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBarWidget.generated.h"

UCLASS()
class ROOM_API UEnemyHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "EnemyHealthBar")
	void UpdateHealth(float HealthRatio);
};
