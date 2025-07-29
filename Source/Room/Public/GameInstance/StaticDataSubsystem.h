#pragma once
//로딩 매니저에서 로드하여 사용
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "StaticDataStruct.h"
#include "StaticDataSubsystem.generated.h"

UCLASS()
class ROOM_API UStaticDataSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
 
    UPROPERTY(EditDefaultsOnly, Category = "DataTable")
    TObjectPtr<UDataTable> ItemDataTable;
    UPROPERTY(EditDefaultsOnly, Category = "DataTable")
    TObjectPtr<UDataTable> InventoryDataTable;
    UPROPERTY(EditDefaultsOnly, Category = "DataTable")
    TObjectPtr<UDataTable> WeaponDataTable;
    UPROPERTY(EditDefaultsOnly, Category = "DataTable")
    TObjectPtr<UDataTable> BulletDataTable;
    UPROPERTY(EditDefaultsOnly, Category = "DataTable")
    TObjectPtr<UDataTable> EnemyDataTable;
    UPROPERTY(EditDefaultsOnly, Category = "DataTable")
    TObjectPtr<UDataTable> MapSpawnDataTable;

   
    void LoadItemData();
    void LoadInventoryData();
    void LoadWeaponData();
    void LoadBulletData();
    void LoadEnemyData();
    void LoadMapSpawnData();

    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
    TMap<int32, FItemData> ItemDataMap;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
    FInventoryData InventoryData;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
    TMap<int32, FWeaponData> WeaponDataMap;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
    TMap<int32, FBulletData> BulletDataMap;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
    TMap<int32, FEnemyData> EnemyDataMap;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
    TMap<FName, FMapSpawnData> MapSpawnDataMap;


    const FItemData* GetItemData(int32 ItemID) const;
    const FInventoryData& GetInventoryData() const;
    const FWeaponData* GetWeaponData(int32 WeaponID) const;
    const FBulletData* GetBulletData(int32 BulletID) const;
    const FEnemyData* GetEnemyData(int32 EnemyID) const;
    const FMapSpawnData* GetMapSpawnData(FName MapName) const;

	virtual void Deinitialize() override;
	
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