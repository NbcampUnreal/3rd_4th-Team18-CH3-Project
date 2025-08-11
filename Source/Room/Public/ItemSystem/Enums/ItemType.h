#pragma once

#include "CoreMinimal.h"
#include "ItemType.generated.h"

UENUM(BlueprintType)
enum class ItemType : uint8
{
	Weapon,
	HealingItem,
};
