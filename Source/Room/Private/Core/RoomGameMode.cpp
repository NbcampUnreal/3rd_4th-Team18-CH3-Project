// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RoomGameMode.h"

#include "AssetTypeCategories.h"
#include "NavigationSystem.h"
#include "Actor/LevelConnector.h"
#include "Components/WeaponComponent.h"
#include "Core/GameManager.h"
#include "GameFramework/PlayerState.h"
#include "ItemSystem/InventoryComponent/InventoryComponent.h"
#include "ItemSystem/Item/BulletItem/BulletItem.h"
#include "ItemSystem/Item/WeaponItem/WeaponItem.h"
#include "Subsystem/StaticDataSubsystem.h"
#include "Subsystem/ObjectPoolSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Define/GameDefine.h"
#include "Characters/MeleeEnemyCharacter.h"
#include "Characters/RangedEnemyCharacter.h"
#include "Actor/Character/BaseCharacter.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/KismetMathLibrary.h"
#include "StaticData/StaticDataStruct.h"
#include "Subsystem/LoadingSubsystem.h"
#include "UI/UISubsystem.h"
#include "Engine/Engine.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RoomGameMode)

void ARoomGameMode::NotifyActorDead(AActor* DeadActor)
{
	
	if (!RoomGameState || !DeadActor)
	{
		return;
	}

	IGameplayTagAssetInterface* TagIf = Cast<IGameplayTagAssetInterface>(DeadActor);
	if (!TagIf)
	{
		return;
	}

	FGameplayTagContainer OwnedTags;
	TagIf->GetOwnedGameplayTags(OwnedTags);

	if (OwnedTags.HasTag(GameDefine::EnemyTag))
	{
		int32 addScore = 100;

		if (Cast<ARangedEnemyCharacter>(DeadActor))
		{
			++RoomGameState->RangedKillCount;
			addScore = 150;
		}
		else if (Cast<AMeleeEnemyCharacter>(DeadActor))
		{
			++RoomGameState->MeleeKillCount;
			addScore = 100;
		}

		++RoomGameState->RoomKillCount;
		++RoomGameState->TotalKillCount;
		if (RoomGameState->AliveEnemyCount > 0)
		{
			--RoomGameState->AliveEnemyCount;
		}

		RoomGameState->Score += addScore;

		if (UUISubsystem* UI = GetGameInstance()->GetSubsystem<UUISubsystem>())
		{
			UI->UpdateObjective(
				RoomGameState->RangedKillCount,
				RoomGameState->RangedTotal,
				RoomGameState->MeleeKillCount,
				RoomGameState->MeleeTotal);

			UI->UpdateScore(RoomGameState->Score);
			UI->ShowKillMarkerOnHUD();
			if (IsLevelClear())
			{
				OnClearLevel();
			}
		}
	}
	else if (OwnedTags.HasTag(GameDefine::PlayerTag)
		|| DeadActor == UGameplayStatics::GetPlayerPawn(this, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player died → OnEndGame(false)"));
		OnEndGame(false);
		UE_LOG(LogTemp, Warning, TEXT("DeadActor=%s, HasPlayerTag=%d"),
			*GetNameSafe(DeadActor), OwnedTags.HasTag(GameDefine::PlayerTag));
	}
}

void ARoomGameMode::NotifyActorSpawn(AActor* SpawnedActor)
{
	if (!RoomGameState || !SpawnedActor)
	{
		return;
	}

	IGameplayTagAssetInterface* TagIf = Cast<IGameplayTagAssetInterface>(SpawnedActor);
	if (!TagIf)
	{
		return;
	}

	FGameplayTagContainer OwnedTags;
	TagIf->GetOwnedGameplayTags(OwnedTags);

	if (OwnedTags.HasTag(GameDefine::EnemyTag))
	{
		++RoomGameState->AliveEnemyCount;
		if (Cast<ARangedEnemyCharacter>(SpawnedActor))
		{
			++RoomGameState->RangedTotal;
		}
		else if (Cast<AMeleeEnemyCharacter>(SpawnedActor))
		{
			++RoomGameState->MeleeTotal;
		}

		if (UUISubsystem* UI = GetGameInstance()->GetSubsystem<UUISubsystem>())
		{
			UI->UpdateObjective(
				RoomGameState->RangedKillCount,
				RoomGameState->RangedTotal,
				RoomGameState->MeleeKillCount,
				RoomGameState->MeleeTotal
			);
		}
	}
}

bool ARoomGameMode::IsLevelClear()
{	// 클리어 조건에 따라 변경 될 수 있음.
	return RoomGameState->AliveEnemyCount == 0;
}

void ARoomGameMode::StartNewRoom()
{
	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), Found);

	int32 rangedTotal = 0;
	int32 meleeTotal = 0;

	for (AActor* A : Found)
	{
		IGameplayTagAssetInterface* TagIf = Cast<IGameplayTagAssetInterface>(A);
		if (!TagIf)
		{
			continue;
		}

		FGameplayTagContainer OwnedTags;
		TagIf->GetOwnedGameplayTags(OwnedTags);

		if (OwnedTags.HasTag(GameDefine::EnemyTag))
		{
			if (Cast<ARangedEnemyCharacter>(A))
			{
				++rangedTotal;
			}
			else if (Cast<AMeleeEnemyCharacter>(A))
			{
				++meleeTotal;
			}
		}
	}

	RoomGameState->RangedTotal = rangedTotal;
	RoomGameState->MeleeTotal = meleeTotal;
	RoomGameState->AliveEnemyCount = rangedTotal + meleeTotal;

	// 카운터 초기화
	RoomGameState->RangedKillCount = 0;
	RoomGameState->MeleeKillCount = 0;
	RoomGameState->RoomKillCount = 0;

	// UI 갱신
	if (UUISubsystem* UI = GetGameInstance()->GetSubsystem<UUISubsystem>())
	{
		UI->UpdateObjective(
			RoomGameState->RangedKillCount,
			RoomGameState->RangedTotal,
			RoomGameState->MeleeKillCount,
			RoomGameState->MeleeTotal
		);
		UI->UpdateScore(RoomGameState->Score);
	}
} 

void ARoomGameMode::OnClearLevel()
{
	ULoadingSubsystem* LoadingSys = GetGameInstance()->GetSubsystem<ULoadingSubsystem>();
	if (!LoadingSys)
	{
		return;
	}

	UStaticDataSubsystem* StaticSys = GetGameInstance()->GetSubsystem<UStaticDataSubsystem>();
	
	FString PrevLevelString = PreviousLevel->GetWorld()->GetMapName();
	
	const FRoomData* RoomData = nullptr;
	int32 NextDataID = 2;
	if (PreviousRoomData != nullptr)
		NextDataID = PreviousRoomData->ID + 1;

	PreviousRoomData = RoomData = StaticSys->GetData<FRoomData>(NextDataID);

	if (RoomData == nullptr)
	{
		OnEndGame(true);
		return;
	}

	// 로드 완료된 레벨 정렬
	// 여기서 TargetConnector 는 미리 선택해둔 커넥터
	TArray<ALevelConnector*> Connectors;
	FindSomeTargetConnector(PreviousLevel, Connectors); // 임시 함수, 구현 필요
	ALevelConnector* TargetConnector = Connectors[FMath::RandRange(0, Connectors.Num() - 1)];
	
	// 델리게이트: 로드 완료 후 호출할 람다
	FStreamableDelegate OnLoadComplete = FStreamableDelegate::CreateLambda([this, RoomData, TargetConnector]()
	{
		AlignLoadedLevelToConnector(RoomData->Level, TargetConnector);
	});
	LoadingSys->LoadLevel(*RoomData, OnLoadComplete);
}

// 패배하거나 완전이 게임을 클리어 한 시점에 호출.
void ARoomGameMode::OnEndGame(bool bIsClear)
{
	int32 FinalScore = 0;
	if (ARoomGameState* RGS = GetGameState<ARoomGameState>())
	{
		FinalScore = RGS->Score;
	}

	if (UUISubsystem* UI = GetGameInstance()->GetSubsystem<UUISubsystem>())
	{
		UI->ShowGameOver(bIsClear, FinalScore);
	}
	
	// TODO : 캐릭터 사망 방지.
}

void ARoomGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetGameInstance()->GetSubsystem<UUISubsystem>()->ShowHUD();
	RoomGameState = GetGameState<ARoomGameState>();
	PreviousLevel = GetWorld()->PersistentLevel;
	
	InitializeGame();
	InitializeStartingItem();
	StartNewRoom();
}

void ARoomGameMode::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

void ARoomGameMode::InitializeGame()
{
	UStaticDataSubsystem* StaticDataSubsystem = GetGameInstance()->GetSubsystem<UStaticDataSubsystem>();
	UObjectPoolSubsystem* ObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();

	if (!StaticDataSubsystem || !ObjectPoolSubsystem || !RoomGameState)
	{
		return;
	}

	const FName CurrentLevelName = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld()));
	const FRoomData* RoomData = StaticDataSubsystem->GetDataByKey<FRoomData, FName>(CurrentLevelName);
	if (!RoomData)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find RoomData for the current level: %s"), *CurrentLevelName.ToString());
		return;
	}

	for (const FPoolableActorData& PoolableActorData : RoomData->PoolAbleActorInfos)
	{
		ObjectPoolSubsystem->InitializePool(PoolableActorData.ActorClass.Get(), PoolableActorData.InitialPoolSize);
	}
	// UGameManager* GManager = Cast<UGameManager>( GetGameInstance());
	// int32 bulletStaticID = GManager->Config->PlayerPreLoadData.DefaultWeaponData.ID;
	// const FBulletItemData* BulletData = GManager->GetSubsystem<UStaticDataSubsystem>()->GetData<FBulletItemData>(bulletStaticID);
	// ObjectPoolSubsystem->InitializePool(BulletData->ProjectileClass, 40);

	RoomGameState->bIsRoomStarted = true;
	OnStartRoom.Broadcast();
}

void ARoomGameMode::InitializeStartingItem()
{
	const int WeaponIndex = 1;
	
	// 무기 생성 및 데이터 적용.
	auto StartingWeapon = NewObject<UWeaponItem>();
	auto WeaponData = GetWorld()->GetGameInstance()->GetSubsystem<UStaticDataSubsystem>()->GetData<FWeaponItemData>(WeaponIndex);
	if (WeaponData)
	{
		StartingWeapon->SetWeaponItemInfo(*WeaponData);
		FString ItemName = WeaponData->ItemName.ToString();
	}

	// 무기 장착.
	APawn* PlayerCharacter = UGameplayStatics::GetPlayerPawn(this,0);
	PlayerCharacter->FindComponentByClass<UWeaponComponent>()->Equip(StartingWeapon);


	// 탄 생성 및 데이터 적용.
	auto StartingBullet = NewObject<UBulletItem>();
	auto BulletData = GetWorld()->GetGameInstance()->GetSubsystem<UStaticDataSubsystem>()->GetDataByKey<FBulletItemData, int32>(WeaponData->WeaponBulletID);
	// auto BulletData = GetWorld()->GetGameInstance()->GetSubsystem<UStaticDataSubsystem>()->GetData<FBulletItemData>(WeaponIndex);
	if (BulletData)
	{
		StartingBullet->SetBulletItemInfo(*BulletData);
	}
	
	// 인벤 토리에 추가.
	auto Inventory = UGameplayStatics::GetPlayerState(this,0)->FindComponentByClass<UInventoryComponent>();
	if (Inventory)
	{
		Inventory->AddItemToInventory(StartingBullet, 100);
	}

	if (auto* UI = GetGameInstance()->GetSubsystem<UUISubsystem>())
	{
		const int32 BulletID = StartingWeapon->GetWeaponBulletID();
		const int32 Ammo = Inventory ? Inventory->GetBulletCount(BulletID) : 0;

		UI->UpdateWeaponInfo(StartingWeapon->GetItemIcon(), StartingWeapon->GetItemName(), Ammo);

	}
	
}

void ARoomGameMode::FindSomeTargetConnector(ULevel* SubLevel, TArray<ALevelConnector*>& OutConnectors)
{
	for (AActor* Actor : SubLevel->Actors)
	{
		if ( !Actor || Actor->IsHidden()) continue;
		
		if (ALevelConnector* Connector = Cast<ALevelConnector>(Actor))
		{
			OutConnectors.Add(Connector);
		}
	}
}

void ARoomGameMode::AlignLoadedLevelToConnector(
	TSoftObjectPtr<UWorld> LevelAsset,
	ALevelConnector* TargetConnector
)
{
	UWorld* World = GetWorld();
	if (!World || !LevelAsset.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid world or level asset"));
		return;
	}

	bool bSuccess = false;
	ULevelStreamingDynamic* LoadedLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
		World,
		LevelAsset,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		bSuccess
	);
	if (!bSuccess || !LoadedLevel)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load streaming level from asset: %s"), *LevelAsset.ToString());
		return;
	}
	LoadedLevel->SetShouldBeVisible(false);
	
	NextLoadedLevel = LoadedLevel;
	PrevLevelConnector = TargetConnector;

	LoadedLevel->OnLevelLoaded.AddDynamic(this, &ARoomGameMode::OnStreamedLevelLoadedHelper);
}

void ARoomGameMode::OnStreamedLevelLoadedHelper()
{
	ULevel* SubLevel = NextLoadedLevel->GetLoadedLevel();
	if (!SubLevel)
	{
		UE_LOG(LogTemp, Warning, TEXT("AlignLoadedLevelToConnector: LoadedLevel has no ULevel"));
		return;
	}

	TArray<ALevelConnector*> Connectors;
	FindSomeTargetConnector(SubLevel, Connectors);

	if (Connectors.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AlignLoadedLevelToConnector: No connectors found in %s"),
			*NextLoadedLevel->GetWorldAssetPackageName());
		return;
	}

	// 랜덤으로 하나 선택
	ALevelConnector* NewConnector = Connectors[FMath::RandRange(0, Connectors.Num() - 1)];
	
	//FTransform NewTransform = NewConnector->GetActorTransform() *  NextLoadedLevel->LevelTransform; 
	FTransform NewTransform = NewConnector->TransformCache;// *  NextLoadedLevel->LevelTransform; 
	// TargetConnector의 월드 트랜스폼
	FTransform TargetTransform(PrevLevelConnector->GetActorRotation(),
							   PrevLevelConnector->GetActorLocation());
	PrevLevelConnector->SetActorHiddenInGame(true);
	NewConnector->SetActorHiddenInGame(true);

	// 2. New → Target 회전 Delta
	FQuat NewQuat = NewTransform.GetRotation();
	FVector TargetForward = TargetTransform.GetRotation().GetForwardVector();
	FVector LookTargetForward = -TargetForward;
	FQuat TargetQuat = FRotationMatrix::MakeFromX(LookTargetForward).ToQuat();
	FQuat DeltaQuat = TargetQuat * NewQuat.Inverse();

	// 3. 피벗(NewTransform 위치) 기준 회전 적용
	FVector Pivot = NewTransform.GetLocation();
	FVector RotatedPivotOffset = DeltaQuat.RotateVector(-Pivot);

	// 4. 위치 보정
	FVector TargetLoc = TargetTransform.GetLocation();
	FVector DeltaTranslation = TargetLoc + RotatedPivotOffset;

	// 5. 최종 DeltaTransform
	FTransform DeltaTransform(DeltaQuat, DeltaTranslation);

	// 레벨 적용
	NextLoadedLevel->LevelTransform = DeltaTransform;
	
	NextLoadedLevel->SetShouldBeVisible(true);
	// 직전 레벨 갱신
	PreviousLevel = SubLevel;

	UWorld* World = GetWorld();
	
	FVector NewVector = NewTransform.GetLocation();
	FVector Start = NewTransform.GetLocation() + FVector(0,0,500);
	FVector End   = TargetTransform.GetLocation() + FVector(0,0,500);
	DrawDebugLine(
		World,
		Start,
		End,
		FColor::Red,
		/*bPersistentLines=*/true,
		/*LifeTime=*/2.f,
		/*DepthPriority=*/0,
		/*Thickness=*/2.f
	);


	
	NewVector = TargetTransform.GetLocation();
	Start = NewVector + FVector(0,0,500);
	End   = NewVector - FVector(0,0,500);
	
	DrawDebugLine(
		World,
		Start,
		End,
		FColor::Blue,
		/*bPersistentLines=*/true,
		/*LifeTime=*/2.f,
		/*DepthPriority=*/0,
		/*Thickness=*/2.f
	);

	
	NewVector = NewTransform.GetLocation();
	Start = NewVector + FVector(0,0,500);
	End   = NewVector - FVector(0,0,500);
	
	DrawDebugLine(
		World,
		Start,
		End,
		FColor::Green,
		/*bPersistentLines=*/true,
		/*LifeTime=*/2.f,
		/*DepthPriority=*/0,
		/*Thickness=*/2.f
	);


	NewVector = (DeltaTransform * NewTransform).GetLocation() ;
	Start  = NewVector + FVector(0,0,500);
	End = NewVector - FVector(0,0,500);
	
	DrawDebugLine(
		World,
		Start,
		End,
		FColor::Purple,
		/*bPersistentLines=*/true,
		/*LifeTime=*/2.f,
		/*DepthPriority=*/0,
		/*Thickness=*/2.f
	);

}
