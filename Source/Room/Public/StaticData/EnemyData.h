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
    TSoftObjectPtr<USkeletalMesh> Mesh = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
     TSoftClassPtr<UAnimInstance> AnimBP = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
    TSoftClassPtr<UBehaviorTree> Behavior = nullptr;
};
