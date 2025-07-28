#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "StaticDataSubsystem.generated.h"
/*class UanimInstance;
class UbehaviorTree;*/

USTRUCT(BlueprintType)
struct FStatData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
    float HP;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
    float Attack;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
    float Defense;
};

USTRUCT(BlueprintType)
struct FItemData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    int32 ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    UStaticMesh* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    UMaterialInterface* Material;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    TSubclassOf<AActor> EffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    int32 MaxSlotSize;
};

USTRUCT(BlueprintType)
struct FBulletData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
    int32 BulletID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
    UStaticMesh* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
    UTexture2D* Icon;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    int32 WeaponID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    float FireRate;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    float Spread;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    UStaticMesh* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    int32 BulletID;
};



USTRUCT(BlueprintType)
struct FMonsterData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
    int32 MonsterID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
    FStatData Stat;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
    USkeletalMesh* Mesh;

   /* UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
    TSubclassOf<UAnimInstance> AnimBP;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
    TSubclassOf<UBehaviorTree> Behavior;*/
};

UENUM(BlueprintType)
enum class EClearCondition : uint8
{
    KillTargetCount,
    SurviveForTime,
    GetItem
};

UCLASS()
class ROOM_API UStaticDataSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    
    const FItemData* GetItemData(int32 ItemID) const;
    const FWeaponData* GetWeaponData(int32 WeaponID) const;
    const FBulletData* GetBulletData(int32 BulletID) const;
    const FMonsterData* GetMonsterData(int32 MonsterID) const;
    
    TMap<int32, FItemData> ItemDataMap;
    TMap<int32, FWeaponData> WeaponDataMap;
    TMap<int32, FBulletData> BulletDataMap;
    TMap<int32, FMonsterData> MonsterDataMap;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> CrouchAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> FireAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> AimAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> ToggleViewAction;
};
