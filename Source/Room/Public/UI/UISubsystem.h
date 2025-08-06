#pragma once

#include "CoreMinimal.h"
#include "Subsystem/RoomSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UISubsystem.generated.h"

UCLASS()
class ROOM_API UUISubsystem : public URoomSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
