#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SoundData.Generated.h"

USTRUCT(BlueprintType)
struct FSoundData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ID")
	int32 ID = 0;
};

USTRUCT(BlueprintType)
struct FBGMData : public FSoundData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	float FadeInDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	float FadeOutDuration = 1.0f;
};