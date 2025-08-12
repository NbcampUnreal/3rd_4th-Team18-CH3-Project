#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"
#include "WeaponData.h"
#include "PlayerPreLoadData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerPreLoadData : public FStaticData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponData DefaultWeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<APawn> PlayerPawn;
};
