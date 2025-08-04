
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RoomSubsystem.generated.h"

class UGameManager;

UCLASS(Abstract)
class ROOM_API URoomSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UGameManager* GetGameManager() const;
};
