#include "Actor/SpawnPoint.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DataTable.h"
#include "StaticData/EnemyData.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITOR
	EditorMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EditorMeshComponent"));
	SetRootComponent(EditorMeshComponent);

	if (!IsTemplate() && !IsRunningCommandlet())
    {
        BeginPIEDelegateHandle = FEditorDelegates::BeginPIE.AddUObject(this, &ASpawnPoint::HandleBeginPIE);
        EndPIEDelegateHandle = FEditorDelegates::EndPIE.AddUObject(this, &ASpawnPoint::HandleEndPIE);
    }
#endif
}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpawnPoint::Destroyed()
{
    Super::Destroyed();
#if WITH_EDITOR
    if (!IsRunningCommandlet())
    {
        FEditorDelegates::BeginPIE.Remove(BeginPIEDelegateHandle);
        FEditorDelegates::EndPIE.Remove(EndPIEDelegateHandle);
    }
#endif
}

#if WITH_EDITOR
void ASpawnPoint::UpdateEditorMesh()
{
    if (!EnemyDataTable.IsValid())
    {   // 반환값을 무시했음을 나타냄.
        (void)EnemyDataTable.LoadSynchronous();
    }

    UDataTable* DataTable = EnemyDataTable.Get();
    if (!DataTable)
    {
        EditorMeshComponent->SetSkeletalMesh(nullptr);
        return;
    }

    const FEnemyData* EnemyData = DataTable->FindRow<FEnemyData>(*FString::FromInt(SpawnDataID), TEXT(""));
    if (!EnemyData)
    {
        EditorMeshComponent->SetSkeletalMesh(nullptr);
        return;
    }

    if (EnemyData->Mesh.IsPending())
    {
        EnemyData->Mesh.LoadSynchronous();
    }
    
    EditorMeshComponent->SetSkeletalMesh(EnemyData->Mesh.Get());
}

void ASpawnPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
    UpdateEditorMesh();
}

void ASpawnPoint::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ASpawnPoint, SpawnDataID) || PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ASpawnPoint, EnemyDataTable))
	{
		UpdateEditorMesh();
	}
}

void ASpawnPoint::HandleBeginPIE(const bool bIsSimulating)
{
    if (EditorMeshComponent)
    {
        EditorMeshComponent->SetVisibility(false);
    }
}

void ASpawnPoint::HandleEndPIE(const bool bIsSimulating)
{
    if (EditorMeshComponent)
    {
        EditorMeshComponent->SetVisibility(true);
    }
}
#endif