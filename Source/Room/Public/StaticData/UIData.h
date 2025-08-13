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
	TSoftClassPtr<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftClassPtr<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftClassPtr<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftClassPtr<UUserWidget> LoadingScreenWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftClassPtr<class ADamageTextActor> DamageTextActorClass;
};
