
#include "Room/Public/ItemSystem/InventoryComponent/InventoryComponent.h"
#include "ItemSystem/Structure/InventorySlot.h"
#include "ItemSystem/Item/ItemBase/ItemBase.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InventorySlots.SetNum(InventorySize);
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

int32 UInventoryComponent::AddStackableItem(UItemBase* NewItem, int32 Quantity)
{
	int32 MaxStackCount = NewItem->GetMaxStackCount();

	// 기존 스택 채우기
	for (FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.Item == nullptr) continue;
		if (Slot.Item->GetItemID() == NewItem->GetItemID() && Slot.Quantity < MaxStackCount)
		{
			int32 Addable = FMath::Min(MaxStackCount - Slot.Quantity, Quantity);
			Slot.Quantity += Addable;
			Quantity -= Addable;

			if (Quantity <= 0)
			{
				return 0;
			}
		}
	}

	// 남은 수량은 빈 슬롯에 새로 추가
	for (FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.Item == nullptr)
		{
			int32 Addable = FMath::Min(MaxStackCount, Quantity);
			Slot.Item = NewItem;
			Slot.Quantity = Addable;
			Quantity -= Addable;

			if (Quantity <= 0)
			{
				return 0;
			}
		}
	}

	if (Quantity > 0)
	{
		return Quantity;
	}

	return 0;

}

int32 UInventoryComponent::AddNonStackableItem(UItemBase* NewItem)
{
	for (FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.Item == nullptr)
		{
			Slot.Item = NewItem;
			Slot.Quantity = 1;
			return 0;
		}
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
	Slot.Quantity -= Dropped;

	if (Slot.Quantity <= 0)
	{
		Slot.Item = nullptr;
		Slot.Quantity = 0;
	}

	//TODO: 드롭한 아이템 월드에 추가.

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

	Slot.Item = nullptr;
	Slot.Quantity = 0;

	//TODO: 드롭한 아이템 월드에 추가.
	
	return true;
}
