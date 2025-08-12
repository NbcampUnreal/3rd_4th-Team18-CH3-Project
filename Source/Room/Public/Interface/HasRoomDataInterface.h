#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HasRoomDataInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHasRoomDataInterface :  public UInterface
{
	GENERATED_BODY()
};

class ROOM_API IHasRoomDataInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RoomData")
	TArray<TSoftObjectPtr<UObject>> GetPreloadAssetList();
	
};
