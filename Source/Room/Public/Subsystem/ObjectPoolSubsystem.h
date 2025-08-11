// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ObjectPoolSubsystem.generated.h"

class IPoolableInterface;

USTRUCT()
struct FActorPool
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TArray<TObjectPtr<AActor>> PooledActors;
};

/**
 * 자주 생성되고, 제거 되는데 동일한 Actor를 사용하는 오브젝트들.
 * 총알
 */
UCLASS()
class ROOM_API UObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	virtual void Deinitialize() override;

public:
	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
	void InitializePool(const TSubclassOf<AActor> PooledActorClass, int32 PoolSize);

	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
	AActor* GetPooledObject(TSubclassOf<AActor> PooledActorClass, const FTransform& SpawnTransform);

	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
	void ReturnPooledObject(AActor* ActorToReturn);

private:
	UPROPERTY()
	TMap<TSubclassOf<AActor>, FActorPool> Pool;

	AActor* CreateNewObject(TSubclassOf<AActor> PooledActorClass);
};