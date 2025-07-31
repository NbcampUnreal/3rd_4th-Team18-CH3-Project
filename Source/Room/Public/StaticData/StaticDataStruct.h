#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"    
#include "Animation/AnimInstance.h" 
#include "BehaviorTree/BehaviorTree.h" 
#include "StaticDataStruct.generated.h" 

//다양한 데이터를 데이터 테이블로 관리하기 위한 구조체들을 정의
USTRUCT(BlueprintType)
struct FStaticData : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ID")
    int32 ID = 0;

};

USTRUCT(BlueprintType)
struct FStatData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
    float HP = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
    float Attack = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
    float Defense = 0.0f;
};

USTRUCT(BlueprintType)
struct FItemData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSoftObjectPtr<UStaticMesh> Mesh = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSoftObjectPtr<UMaterialInterface> Material = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSoftClassPtr<AActor> EffectClass = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    int32 MaxStack = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    int32 ItemCount = 0;
};

USTRUCT(BlueprintType)
struct FInventoryData : public FStaticData
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    TMap<int32, int32> ItemList;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    int32 MaxSlotSize = 0;
};

USTRUCT(BlueprintType)
struct FBulletData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
    float Speed = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
    TSoftObjectPtr<UStaticMesh> Mesh = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
    TSoftObjectPtr<UTexture2D> Icon = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float Damage = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float FireRate = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float Spread = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TSoftObjectPtr<UStaticMesh> Mesh = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    int32 BulletID = 0;
};


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

USTRUCT(BlueprintType)
struct FEnemySpawnData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    int32 EnemyID = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    int32 Count = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    float SpawnTime = 0.0f;
};

USTRUCT(BlueprintType)
struct FItemSpawnData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    int32 ItemID = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    float SpawnProbability = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    float SpawnTime = 0.0f;
};

USTRUCT(BlueprintType)
struct FMapSpawnData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
    TArray<FEnemySpawnData> EnemySpawns;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
    TArray<FItemSpawnData> ItemSpawns;
};

UENUM(BlueprintType)
enum class EClearCondition : uint8
{
    KillTargetCount,
    SurviveForTime,
    GetItem
};