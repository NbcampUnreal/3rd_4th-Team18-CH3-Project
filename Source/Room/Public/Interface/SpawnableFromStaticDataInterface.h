
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpawnableFromStaticDataInterface.generated.h"

struct FStaticData;

UINTERFACE(MinimalAPI, Blueprintable)
class USpawnableFromStaticDataInterface : public UInterface
{
	GENERATED_BODY()
};

class ROOM_API ISpawnableFromStaticDataInterface
{
	GENERATED_BODY()

public:
	virtual void InitializeFromStaticData(const FStaticData* InStaticData) = 0;
};
