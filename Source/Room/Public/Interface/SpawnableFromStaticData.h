
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpawnableFromStaticData.generated.h"

struct FStaticData;

UINTERFACE(MinimalAPI, Blueprintable)
class USpawnableFromStaticData : public UInterface
{
	GENERATED_BODY()
};

class ROOM_API ISpawnableFromStaticData
{
	GENERATED_BODY()

public:
	virtual void InitializeFromStaticData(const FStaticData* InStaticData) = 0;
};
