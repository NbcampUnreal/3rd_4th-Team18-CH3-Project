#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"
#include "StatData.h"
#include "Animation/AnimInstance.h" 
#include "BehaviorTree/BehaviorTree.h" 
#include "EnemyData.generated.h"

USTRUCT(BlueprintType)
struct FEnemyData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
    FStatData Stat;

	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
    TSoftObjectPtr<UBehaviorTree> Behavior = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
	TSoftClassPtr<AActor> EnemyClass = nullptr;
};
