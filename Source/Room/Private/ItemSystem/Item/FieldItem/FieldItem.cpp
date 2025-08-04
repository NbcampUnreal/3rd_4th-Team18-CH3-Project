
#include "ItemSystem/Item/FieldItem/FieldItem.h"

#include "ItemSystem/Item/ItemBase/ItemBase.h"

AFieldItem::AFieldItem()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->SetCollisionProfileName(TEXT("Item"));
}

void AFieldItem::Interact_Implementation()
{
	IInteractable::Interact_Implementation();
	Pickuped();
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
	
	// return FText::FromString(PickString);
	return Item->GetItemDescription();
}

void AFieldItem::Pickuped()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Pickuped!");
	Destroy();
}

