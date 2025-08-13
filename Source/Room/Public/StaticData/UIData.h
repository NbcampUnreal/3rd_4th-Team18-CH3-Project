#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"
#include "UI/Widget/DamageTextActor.h"
#include "GameFramework/Actor.h"
#include "UIData.generated.h"

USTRUCT(BlueprintType)
struct FUIData : public FStaticData
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftClassPtr<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftClassPtr<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftClassPtr<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftClassPtr<UUserWidget> LoadingScreenWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftClassPtr<ADamageTextActor> DamageTextActorClass;
};
