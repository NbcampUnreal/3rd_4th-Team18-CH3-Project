
#include "Subsystem/RoomSubsystem.h"
#include "Core/GameManager.h"

UGameManager* URoomSubsystem::GetGameManager() const
{
    return Cast<UGameManager>(GetGameInstance());
}
