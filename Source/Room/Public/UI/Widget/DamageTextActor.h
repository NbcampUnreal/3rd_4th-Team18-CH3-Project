#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTextActor.generated.h"

class UWidgetComponent;

UCLASS()
class ROOM_API ADamageTextActor : public AActor
{
	GENERATED_BODY()

public:
	ADamageTextActor();

	void SetDamage(int32 Damage);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* WidgetComponent;
};