// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/RoomGameState.h"
#include "RoomGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartRoomSignature);

/**
 * Room 하나의 규칙에 대한 GameMode
 * 모든적 처치시 다음 레벨.
 * 플레이어 사망시 GameOver
 */
UCLASS()
class ROOM_API ARoomGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<ARoomGameState> RoomGameState; // 캐시
	
public:

	UPROPERTY(BlueprintAssignable, Category = "Room Game")
	FOnStartRoomSignature OnStartRoom;

	virtual void NotifyActorDead(AActor* DeadActor);
	virtual void NotifyActorSpawn(AActor* SpawnedActor);
	virtual bool IsLevelClear();
	virtual void StartNewRoom();
private:
	virtual void OnClearLevel();

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	void InitializeGame();
	
};
