#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HasRoomDataInterface.h"
#include "Interface/SpawnableFromStaticData.h"
#include "SpawnPoint.generated.h"

UCLASS()
class ROOM_API ASpawnPoint : public AActor, public IHasRoomDataInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void UpdateEditorMesh();

	void HandleBeginPIE(const bool bIsSimulating);
	void HandleEndPIE(const bool bIsSimulating);
	
	FDelegateHandle BeginPIEDelegateHandle;
	FDelegateHandle EndPIEDelegateHandle;
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Spawn|Editor")
	TSoftObjectPtr<class UDataTable> EnemyDataTable;
	UPROPERTY()
	TObjectPtr<class USkeletalMeshComponent> EditorMeshComponent;
#endif
	
#endif

public:
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	virtual AActor* PerformSpawnActor();

	// New properties for data-driven spawning
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (MustImplement = "SpawnableFromStaticData"))
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (MetaClass = "StaticData"))
	TObjectPtr<UScriptStruct> DataType;

	// Existing properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 SpawnDataID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSoftObjectPtr<AActor> SpawnActor;

	TArray<TSoftObjectPtr<UObject>> GetPreloadAssetList_Implementation() override;
};
