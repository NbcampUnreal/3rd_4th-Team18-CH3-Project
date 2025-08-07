#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"
#include "GameFramework/Actor.h"
#include "UIData.generated.h"

USTRUCT(BlueprintType)
struct FUIData : public FStaticData
{
    GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSoftClassPtr<UUserWidget> WidgetClass;
};
