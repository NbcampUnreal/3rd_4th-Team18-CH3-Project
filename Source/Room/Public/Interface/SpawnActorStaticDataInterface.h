#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/SpawnableFromStaticDataInterface.h"
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
	// 조건 체크용 템플릿

	virtual TSubclassOf<AActor> SpawnActorClass() = 0;
};
