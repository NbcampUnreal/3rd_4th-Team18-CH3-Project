
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Equipper.generated.h"

UINTERFACE()
class UEquipper : public UInterface
{
	GENERATED_BODY()
};

class ROOM_API IEquipper
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	void Equip(UObject* Equipment);
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	void Unequip();
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	bool IsEquipping() const;
};
