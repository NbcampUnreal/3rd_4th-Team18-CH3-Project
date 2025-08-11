
#include "Room/Public/ItemSystem/InventoryComponent/InventoryComponent.h"

#include "ItemSystem/Item/FieldItem/FieldItem.h"
#include "ItemSystem/Structure/InventorySlot.h"
#include "ItemSystem/Item/ItemBase/ItemBase.h"
#include "Kismet/GameplayStatics.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InventorySlots.SetNum(InventorySize);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

int32 UInventoryComponent::AddItemToInventory(UItemBase* NewItem, int32 Quantity)
{
	if (!NewItem || Quantity <= 0)
	{
		return Quantity;
	}

	if (NewItem->IsConsumable())
	{
		return AddStackableItem(NewItem, Quantity);
	}
	else
	{
		return AddNonStackableItem(NewItem);
	}
}

bool UInventoryComponent::UseItem(int32 SlotIndex)
{
	if (!InventorySlots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	FInventorySlot& Slot = InventorySlots[SlotIndex];
	if (Slot.Item == nullptr)
	{
		return false;
	}

	if (Slot.Item->IsConsumable())
	{
		return UseStackableItem(SlotIndex);
	}
	else
	{
		return UseNonStackableItem(SlotIndex);
	}
}

bool UInventoryComponent::DropItemFromInventory(int32 SlotIndex, int32 Quantity)
{
	if (Quantity <= 0)
	{
		return false;
	}
	
	if (!InventorySlots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	FInventorySlot& Slot = InventorySlots[SlotIndex];
	if (Slot.Item == nullptr)
	{
		return false;
	}

	if (Slot.Item->IsConsumable())
	{
		return DropStackableItem(SlotIndex,Quantity);
	}
	else
	{
		return DropNonStackableItem(SlotIndex);
	}
}

const FInventorySlot& UInventoryComponent::GetInventorySlotByIndex(int32 SlotIndex) const
{
	return InventorySlots[SlotIndex];
}

int32 UInventoryComponent::AddStackableItem(UItemBase* NewItem, int32 Quantity)
{
	int32 MaxStackCount = NewItem->GetMaxStackCount();

	// 기존 스택 채우기
	int SlotIndex = 0;
	for (FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.Item == nullptr) continue;
		if (Slot.Item->GetItemID() == NewItem->GetItemID() && Slot.Quantity < MaxStackCount)
		{
			int32 Addable = FMath::Min(MaxStackCount - Slot.Quantity, Quantity);
			Slot.Quantity += Addable;
			Quantity -= Addable;
			OnSlotChanged.Broadcast();
			
			if (Quantity <= 0)
			{
				return 0;
			}
		}
		SlotIndex++;
	}

	SlotIndex=0;
	// 남은 수량은 빈 슬롯에 새로 추가
	for (FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.Item == nullptr)
		{
			int32 Addable = FMath::Min(MaxStackCount, Quantity);
			Slot.Item = NewItem;
			Slot.Quantity = Addable;
			Quantity -= Addable;
			OnSlotChanged.Broadcast();

			if (Quantity <= 0)
			{
				return Quantity;
			}
		}
		SlotIndex++;
	}

	return Quantity;

}

int32 UInventoryComponent::AddNonStackableItem(UItemBase* NewItem)
{
	int32 SlotIndex = 0;
	for (FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.Item == nullptr)
		{
			Slot.Item = NewItem;
			Slot.Quantity = 1;
			OnSlotChanged.Broadcast();
			return 0;
		}
		SlotIndex++;
	}

	return 1;
}

bool UInventoryComponent::UseStackableItem(int32 SlotIndex)
{
	if (!InventorySlots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	FInventorySlot& Slot = InventorySlots[SlotIndex];
	if (Slot.Item == nullptr || Slot.Quantity <= 0)
	{
		return false;
	}

	Slot.Item->Use(GetOwner());
	Slot.Quantity--;
	
	if (Slot.Quantity <= 0)
	{
		Slot.Item = nullptr;
		Slot.Quantity = 0;
	}
	OnSlotChanged.Broadcast();

	return true;
}

bool UInventoryComponent::UseNonStackableItem(int32 SlotIndex)
{
	if (!InventorySlots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	FInventorySlot& Slot = InventorySlots[SlotIndex];
	if (Slot.Item == nullptr)
	{
		return false;
	}

	Slot.Item->Use(GetOwner());
	Slot.Item = nullptr;
	Slot.Quantity = 0;
	OnSlotChanged.Broadcast();
	return true;
}

bool UInventoryComponent::DropStackableItem(int32 SlotIndex, int32 Quantity)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("DropStackableItem"));
	if (!InventorySlots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	FInventorySlot& Slot = InventorySlots[SlotIndex];
	if (Slot.Item == nullptr || Slot.Quantity <= 0)
	{
		return false;
	}

	int32 Dropped = FMath::Min(Quantity, Slot.Quantity);
	
	FVector SpawnLocation = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();
	AFieldItem* SpawnedItem = GetWorld()->SpawnActor<AFieldItem>(AFieldItem::StaticClass(),SpawnLocation,FRotator::ZeroRotator);
	SpawnedItem->SetItem(Slot.Item);
	SpawnedItem->SetQuantity(Dropped);
	SpawnedItem->SetItemMesh(Slot.Item->GetItemMesh());

	
	Slot.Quantity -= Dropped;

	if (Slot.Quantity <= 0)
	{
		Slot.Item = nullptr;
		Slot.Quantity = 0;
	}
	OnSlotChanged.Broadcast();

	
	
	return true;
}


bool UInventoryComponent::DropNonStackableItem(int32 SlotIndex)
{
	if (!InventorySlots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	FInventorySlot& Slot = InventorySlots[SlotIndex];
	if (Slot.Item == nullptr)
	{
		return false;
	}

	FVector SpawnLocation = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();
	AFieldItem* SpawnedItem = GetWorld()->SpawnActor<AFieldItem>(AFieldItem::StaticClass(),SpawnLocation,FRotator::ZeroRotator);
	SpawnedItem->SetItem(Slot.Item);
	SpawnedItem->SetQuantity(1);
	SpawnedItem->SetItemMesh(Slot.Item->GetItemMesh());

	
	Slot.Item = nullptr;
	Slot.Quantity = 0;
	OnSlotChanged.Broadcast();
	return true;
}

