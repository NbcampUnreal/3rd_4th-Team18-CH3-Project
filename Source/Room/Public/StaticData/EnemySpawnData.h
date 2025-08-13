#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StaticData.h" // FStaticData를 포함하기 위해 추가
#include "EnemySpawnData.generated.h"

UENUM(BlueprintType)
enum class ESpawnTriggerCondition : uint8
{
	OnGameStart UMETA(DisplayName = "On Game Start"),
	OnWaveClear UMETA(DisplayName = "On Wave Clear"),
	OnPlayerEnterTrigger UMETA(DisplayName = "On Player Enter Trigger"),
};

USTRUCT(BlueprintType)
struct FEnemySpawnData : public FStaticData // FTableRowBase 대신 FStaticData 상속
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FDataTableRowHandle EnemyID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	ESpawnTriggerCondition SpawnTriggerCondition;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	// float SpawnDelay;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	// int32 Wave;
};