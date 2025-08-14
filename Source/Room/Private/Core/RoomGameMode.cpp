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
#include "StaticData/StaticDataStruct.h"
#include "UI/UISubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RoomGameMode)

void ARoomGameMode::NotifyActorDead(AActor* DeadActor)
{
	// 사망 카운트
	// 플레이어거나, 적이냐에 따라서 다르게 작동.
}

void ARoomGameMode::NotifyActorSpawn(AActor* SpawnedActor)
{
	// 플레이어이거나 적이다.
	// 적이 스폰됐다면 GameState를 업데이트 한다.
}

bool ARoomGameMode::IsLevelClear()
{
	return false;
}

void ARoomGameMode::StartNewRoom()
{
	// 모든 캐릭터 베이스 가져와서 적 태그인것만 사용.
	//RoomGameState->AliveEnemyCount =  
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
