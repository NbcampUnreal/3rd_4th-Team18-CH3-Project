#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"    
#include "Animation/AnimInstance.h" 
#include "BehaviorTree/BehaviorTree.h" 
#include "StaticDataStruct.generated.h" 


USTRUCT(BlueprintType)
struct FStatData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
    float HP;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
    float Attack;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
    float Defense;
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    int32 ItemID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSoftObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSoftObjectPtr<UMaterialInterface> Material;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSoftClassPtr<AActor> EffectClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    int32 MaxSlotSize;
};

USTRUCT(BlueprintType)
struct FInventoryData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    TArray<int32> ItemList;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    int32 ItemCount;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    int32 MaxSlotSize;
    
};

USTRUCT(BlueprintType)
struct FBulletData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
    int32 BulletID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
    float Speed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
    TSoftObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
    TSoftObjectPtr<UTexture2D> Icon;
};

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    int32 WeaponID;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float Damage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float FireRate;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float Spread;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TSoftObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    int32 BulletID;
};


USTRUCT(BlueprintType)
struct FEnemyData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
    int32 EnemyID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
    FStatData Stat;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
    TSoftObjectPtr<USkeletalMesh> Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
     TSoftClassPtr<UAnimInstance> AnimBP;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
    TSoftClassPtr<UBehaviorTree> Behavior;
};

USTRUCT(BlueprintType)
struct FEnemySpawnData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    FVector Location;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    int32 EnemyID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    int32 Count;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    float SpawnTime;
};

USTRUCT(BlueprintType)
struct FItemSpawnData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    FVector Location;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    int32 ItemID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    float SpawnProbability;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    float SpawnTime;
};

USTRUCT(BlueprintType)
struct FMapSpawnData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
    FName MapName;

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
