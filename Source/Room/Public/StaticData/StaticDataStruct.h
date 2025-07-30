#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"    
#include "Animation/AnimInstance.h" 
#include "BehaviorTree/BehaviorTree.h" 
#include "StaticDataStruct.generated.h" 
//다양한 데이터를 데이터 테이블로 관리하기 위한 구조체들을 정의
USTRUCT(BlueprintType)
struct FStaticDataBase : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ID")
    int32 ID; 

};

USTRUCT(BlueprintType)
struct FStatData : public FStaticDataBase
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
struct FItemData : public FStaticDataBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSoftObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSoftObjectPtr<UMaterialInterface> Material;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSoftClassPtr<AActor> EffectClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    int32 MaxStack;
};

USTRUCT(BlueprintType)
struct FInventoryData : public FStaticDataBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    TMap<int32, int32> ItemList; //ItemID, count
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    int32 MaxSlotSize;
    //아이템 리스트 추가??
};

USTRUCT(BlueprintType)
struct FBulletData : public FStaticDataBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
    float Speed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
    TSoftObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
    TSoftObjectPtr<UTexture2D> Icon;
};

USTRUCT(BlueprintType)
struct FWeaponData : public FStaticDataBase
{
    GENERATED_BODY()

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
struct FEnemyData : public FStaticDataBase
{
    GENERATED_BODY()

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
struct FEnemySpawnData : public FStaticDataBase
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
struct FItemSpawnData : public FStaticDataBase
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
struct FMapSpawnData : public FStaticDataBase
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
