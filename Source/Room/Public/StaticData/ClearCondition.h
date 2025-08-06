#pragma once

#include "CoreMinimal.h"
#include "ClearCondition.generated.h"

UENUM(BlueprintType)
enum class EClearCondition : uint8
{
    KillTargetCount,
    SurviveForTime,
    GetItem
};
