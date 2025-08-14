// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RoomGameMode.h"

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
#include "StaticData/StaticDataStruct.h"
#include "UI/UISubsystem.h"

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

			if (IsLevelClear())
			{
				OnClearLevel();
			}
		}
		else if (OwnedTags.HasTag(GameDefine::PlayerTag))
		{
			// Player Death Logic
			// TODO : after GameOver HUD
		}
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
{
	return false;
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
	
}

void ARoomGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetGameInstance()->GetSubsystem<UUISubsystem>()->ShowHUD();
	RoomGameState = GetGameState<ARoomGameState>();
	InitializeGame();
	InitializeStartingItem();
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
	
}
