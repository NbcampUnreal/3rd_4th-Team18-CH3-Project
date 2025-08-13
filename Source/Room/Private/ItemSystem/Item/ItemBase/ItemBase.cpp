// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSystem/Item/ItemBase/ItemBase.h"

void UItemBase::Use_Implementation(AActor* User)
{
}


FName UItemBase::GetItemID_Implementation() const
{
	return FName(TEXT(""));
}

FName UItemBase::GetItemName_Implementation() const
{
	return FName(TEXT(""));
}

FText UItemBase::GetItemDescription_Implementation() const
{
	return FText::GetEmpty();
}

UTexture2D* UItemBase::GetItemIcon_Implementation() const
{
	return nullptr;
}

UStaticMesh* UItemBase::GetItemMesh_Implementation() const
{
	return nullptr;
}

bool UItemBase::IsConsumable_Implementation() const
{
	return false;
}

int32 UItemBase::GetMaxStackCount_Implementation() const
{
	return 9;
}

bool UItemBase::IsUsable_Implementation() const
{
	return true;
}
