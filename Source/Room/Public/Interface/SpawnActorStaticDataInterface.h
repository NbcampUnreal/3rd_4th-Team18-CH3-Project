#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpawnActorStaticDataInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USpawnActorStaticDataInterface : public UInterface
{
	GENERATED_BODY()
};

class ISpawnActorStaticDataInterface
{
	GENERATED_BODY()
	
public:
	virtual TSubclassOf<AActor> SpawnActorClass() = 0;
};
