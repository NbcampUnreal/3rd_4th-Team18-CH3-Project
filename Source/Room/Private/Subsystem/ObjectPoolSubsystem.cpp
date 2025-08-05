// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ObjectPoolSubsystem.h"
#include "Interface/PoolableInterface.h"

void UObjectPoolSubsystem::Deinitialize()
{
    for (auto& Elem : Pool)
    {
        for (AActor* Actor : Elem.Value.PooledActors)
        {
            if (Actor)
            {
                Actor->Destroy();
            }
        }
    }
    Pool.Empty();

    Super::Deinitialize();
}

void UObjectPoolSubsystem::InitializePool(TSubclassOf<AActor> PooledActorClass, int32 PoolSize)
{
	if (!PooledActorClass)
	{
		return;
	}

	FActorPool& ActorPool = Pool.FindOrAdd(PooledActorClass);
	for (int32 i = 0; i < PoolSize; ++i)
	{
		AActor* NewActor = CreateNewObject(PooledActorClass);
		if (NewActor)
		{
			ActorPool.PooledActors.Add(NewActor);
		}
	}
}

AActor* UObjectPoolSubsystem::GetPooledObject(TSubclassOf<AActor> PooledActorClass, const FTransform& SpawnTransform)
{
	if (!PooledActorClass)
	{
		return nullptr;
	}

	FActorPool* ActorPoolPtr = Pool.Find(PooledActorClass);
	AActor* PooledActor = nullptr;

	if (ActorPoolPtr && !ActorPoolPtr->PooledActors.IsEmpty())
	{
		PooledActor = ActorPoolPtr->PooledActors.Pop();
	}
	else
	{
		// 풀에 사용 가능한 액터가 없으면 새로 생성
		PooledActor = CreateNewObject(PooledActorClass);
	}

	if (PooledActor)
	{
		if (PooledActor->GetClass()->ImplementsInterface(UPoolableInterface::StaticClass()))
		{
			IPoolableInterface::Execute_OnPoolBegin(PooledActor, SpawnTransform);
		}
	}
	
	return PooledActor;
}

void UObjectPoolSubsystem::ReturnPooledObject(AActor* ActorToReturn)
{
	if (!ActorToReturn)
	{
		return;
	}

	TSubclassOf<AActor> ActorClass = ActorToReturn->GetClass();
	FActorPool& ActorPool = Pool.FindOrAdd(ActorClass);

	if (ActorToReturn->GetClass()->ImplementsInterface(UPoolableInterface::StaticClass()))
	{
		IPoolableInterface::Execute_OnPoolEnd(ActorToReturn);
	}
	
	ActorPool.PooledActors.Add(ActorToReturn);
}

AActor* UObjectPoolSubsystem::CreateNewObject(TSubclassOf<AActor> PooledActorClass)
{
	UWorld* World = GetWorld();
	if (!World || !PooledActorClass)
	{
		return nullptr;
	}

	AActor* NewActor = World->SpawnActorDeferred<AActor>(PooledActorClass, FTransform::Identity);
	if (NewActor)
	{
		if (NewActor->GetClass()->ImplementsInterface(UPoolableInterface::StaticClass()))
        {
            IPoolableInterface::Execute_OnPoolEnd(NewActor);
        }
		NewActor->FinishSpawning(FTransform::Identity, true);
	}

	return NewActor;
}
