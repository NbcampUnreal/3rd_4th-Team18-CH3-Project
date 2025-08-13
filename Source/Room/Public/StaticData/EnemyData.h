#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"
#include "StatData.h"
#include "Animation/AnimInstance.h" 
#include "BehaviorTree/BehaviorTree.h" 
#include "Interface/SpawnActorStaticDataInterface.h"
#include "EnemyData.generated.h"

USTRUCT(BlueprintType)
struct FEnemyData : public FStaticData, public ISpawnActorStaticDataInterface
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
    FStatData Stat;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
    TSoftObjectPtr<UBehaviorTree> Behavior = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
	TSoftClassPtr<AActor> EnemyClass = nullptr;
	// 소리나 파티클 등 리소스를 사용하면 추가 정의

	virtual TSubclassOf<AActor> SpawnActorClass() override { return EnemyClass.Get(); }
};
