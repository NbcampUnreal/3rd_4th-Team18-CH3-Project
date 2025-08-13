#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"
#include "GameFramework/Actor.h"
#include "Interface/SpawnActorStaticDataInterface.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FStaticData, public ISpawnActorStaticDataInterface
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSoftClassPtr<AActor> ItemBPClass = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    int32 MaxStack = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    int32 ItemCount = 0;
    
    virtual TSubclassOf<AActor> SpawnActorClass() override { return ItemBPClass.Get(); }
};
