// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/LevelConnector.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LevelConnector) 

// Sets default values
ALevelConnector::ALevelConnector()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

#if WITH_EDITOR
void ALevelConnector::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (!bFinished) return;


	TransformCache = RootComponent->GetComponentTransform();
	UE_LOG(LogTemp, Log, TEXT("TransformCache auto-cached: %s"), *TransformCache.ToString());

#endif
}
