// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RoomGameState.generated.h"

/**
 * 
 */
UCLASS()
class ROOM_API ARoomGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Game")
	int32 TotalKillCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Game")
	int32 RoomKillCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Game")
	int32 AliveEnemyCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Room Game")
	bool bIsRoomStarted = false;
};
