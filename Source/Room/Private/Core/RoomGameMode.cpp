// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomGameMode.h"

void ARoomGameMode::NotifyActorDead(AActor* DeadActor)
{
	// 사망 카운트
	// 플레이어거나, 적이냐에 따라서 다르게 작동.
}

bool ARoomGameMode::IsLevelClear()
{
	return false;
}

void ARoomGameMode::OnClearLevel()
{
	
}
