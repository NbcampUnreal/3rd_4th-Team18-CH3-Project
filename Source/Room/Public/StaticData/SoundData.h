#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SoundData.Generated.h"

class USoundBase;

USTRUCT(BlueprintType)
struct FSoundData : public FStaticData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DefaultVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (ClampMin = "0.1", ClampMax = "3.0"))
	float DefaultPitch = 1.0f;
};