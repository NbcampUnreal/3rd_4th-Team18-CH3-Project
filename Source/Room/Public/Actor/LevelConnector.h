// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelConnector.generated.h"

UCLASS()
class ROOM_API ALevelConnector : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelConnector();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = LevelConnector)
	FTransform TransformCache;

#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;
#endif
protected:
	
};
