#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"
#include "WeaponItemData.h"
#include "PlayerPreLoadData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerPreLoadData : public FStaticData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponItemData DefaultWeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<APawn> PlayerPawn;
};
