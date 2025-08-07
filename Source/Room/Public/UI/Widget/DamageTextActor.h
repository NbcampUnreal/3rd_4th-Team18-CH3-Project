#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTextActor.generated.h"

class UWidgetComponent;
class UDamageNumberWidget;

UCLASS()
class ROOM_API ADamageTextActor : public AActor
{
    GENERATED_BODY()

public:
    ADamageTextActor();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UWidgetComponent* WidgetComponent;

    FTimerHandle DestroyTimer;

public:
    void SetDamageValue(int32 Damage);

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    float DestroyDelay = 1.0f;

    UFUNCTION()
    void DestroySelf();
};