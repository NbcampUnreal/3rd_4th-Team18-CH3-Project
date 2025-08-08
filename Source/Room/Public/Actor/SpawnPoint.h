#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

UCLASS()
class ROOM_API ASpawnPoint : public AActor
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


#endif

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 SpawnDataID;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Spawn|Editor")
	TSoftObjectPtr<class UDataTable> EnemyDataTable;
	FDelegateHandle BeginPIEDelegateHandle;
	FDelegateHandle EndPIEDelegateHandle;
#endif

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<class USkeletalMeshComponent> EditorMeshComponent;
#endif
};
