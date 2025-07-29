// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RoomGameMode.generated.h"

/**
 * Room 하나의 규칙에 대한 GameMode
 * 모든 적 처치시 다음 레벨.
 * 플레이어 사망시 GameOver
 */
UCLASS()
class ROOM_API ARoomGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	
public:

	virtual void NotifyActorDead(AActor* DeadActor);
	virtual bool IsLevelClear();
private:
	virtual void OnClearLevel();
};
