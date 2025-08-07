
#include "ItemSystem/Item/FieldItem/FieldItem.h"

#include "GameFramework/PlayerState.h"
#include "ItemSystem/InventoryComponent/InventoryComponent.h"
#include "ItemSystem/Item/ItemBase/ItemBase.h"
#include "Kismet/GameplayStatics.h"

AFieldItem::AFieldItem()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->SetCollisionProfileName(TEXT("Item"));
}

void AFieldItem::Interact_Implementation(AActor* Caller)
{
	IInteractable::Interact_Implementation(Caller);
	OnPickup(Caller);
}

void AFieldItem::InFocus_Implementation()
{
	IInteractable::InFocus_Implementation();
	ItemMesh->SetOverlayMaterial(OutlineOverlayMaterial);
}

void AFieldItem::OutFocus_Implementation()
{
	IInteractable::OutFocus_Implementation();
	ItemMesh->SetOverlayMaterial(nullptr);
}

FText AFieldItem::GetInteractableMessage_Implementation()
{
	if (!Item) return FText::GetEmpty();
	
	FName ItemName = Item->GetItemName();
	FString PickString = FString::Printf(TEXT("Pickup! ItemName: %s"), *ItemName.ToString());
	
	return Item->GetItemDescription();
}

void AFieldItem::OnPickup(AActor* Caller)
{
	if (Caller)
	{
		auto Inventory = Caller->FindComponentByClass<UInventoryComponent>();
		if (Inventory)
		{
			int32 Remain = Inventory->AddItemToInventory(Item,Quantity);
			Quantity = Remain;
			if (Quantity <= 0)
			{
				Destroy();
			}
		}

	}
	
	// auto PlayerState = UGameplayStatics::GetPlayerState(this,0);
	// if (PlayerState)
	// {
	// 	auto Inventory = PlayerState->FindComponentByClass<UInventoryComponent>();
	// 	int32 Remain = Inventory->AddItemToInventory(Item,Quantity);
	// 	Quantity = Remain;
	// 	if (Quantity <= 0)
	// 	{
	// 		Destroy();
	// 	}
	//
	// }
}

