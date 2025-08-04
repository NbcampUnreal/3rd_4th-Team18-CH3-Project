
#include "ItemSystem/Inventory/InventoryComponent.h"
#include "ItemSystem/Structure/InventorySlot.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InventorySlots.SetNum(InventorySize);
}

bool UInventoryComponent::AddItemToInventory(UItemBase* NewItem, int32 Quantity)
{
	return false;
}

bool UInventoryComponent::UseItem(int32 SlotIndex)
{
	return false;
}

bool UInventoryComponent::DropItemFromInventory(int32 SlotIndex, int32 Quantity)
{
	return false;
}

bool UInventoryComponent::AddStackableItem(UItemBase* NewItem, int32 Quantity)
{
	return false;
}

bool UInventoryComponent::AddNonStackableItem(UItemBase* NewItem)
{
	return false;
}

bool UInventoryComponent::UseStackableItem(int32 SlotIndex)
{
	return false;
}

bool UInventoryComponent::UseNonStackableItem(int32 SlotIndex)
{
	return false;
}

bool UInventoryComponent::DropStackableItem(int32 SlotIndex, int32 Quantity)
{
	return false;
}

bool UInventoryComponent::DropNonStackableItem(int32 SlotIndex)
{
	return false;
}
