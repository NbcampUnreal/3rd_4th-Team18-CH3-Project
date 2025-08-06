#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"
#include "GameFramework/Actor.h"
#include "PoolableActorData.generated.h"

USTRUCT(BlueprintType)
struct FPoolableActorData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pool")
    TSoftClassPtr<AActor> ActorClass = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pool")
    int32 InitialPoolSize = 0;
};
