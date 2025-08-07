#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumberWidget.generated.h"
class UTextBlock;

UCLASS()
class ROOM_API UDamageNumberWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget)) UTextBlock* DamageText;

public:
    void SetDamage(int32 Value);
};