// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RoomGameMode.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RoomGameMode)

void ARoomGameMode::NotifyActorDead(AActor* DeadActor)
{
	// 사망 카운트
	// 플레이어거나, 적이냐에 따라서 다르게 작동.
}

void ARoomGameMode::NotifyActorSpawn(AActor* SpawnedActor)
{
	// 플레이어이거나 적이다.
	// 적이 스폰됐다면 GameState를 업데이트 한다.
}

bool ARoomGameMode::IsLevelClear()
{
	return false;
}

void ARoomGameMode::StartNewRoom()
{
	// 모든 캐릭터 베이스 가져와서 적 태그인것만 사용.
	//RoomGameState->AliveEnemyCount =  
}

void ARoomGameMode::OnClearLevel()
{
	
}

void ARoomGameMode::BeginPlay()
{
	Super::BeginPlay();
	RoomGameState = GetGameState<ARoomGameState>();
}
