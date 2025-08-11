// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSystem/Item/HealingItem/HealingItem.h"

#include "Kismet/GameplayStatics.h"

void UHealingItem::Use_Implementation(AActor* User)
{
	Super::Use_Implementation(User);
	UGameplayStatics::ApplyDamage(User,GetHealingAmount(),nullptr,nullptr,UDamageType::StaticClass());
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Healing %f"),GetHealingAmount()));
}

FName UHealingItem::GetItemID_Implementation() const
{
	return HealingItemInfo.ItemID;
}

FName UHealingItem::GetItemName_Implementation() const
{
	return HealingItemInfo.ItemName;
}

FText UHealingItem::GetItemDescription_Implementation() const
{
	return HealingItemInfo.ItemDescription;
}

UTexture2D* UHealingItem::GetItemIcon_Implementation() const
{
	return HealingItemInfo.ItemIcon;
}

UStaticMesh* UHealingItem::GetItemMesh_Implementation() const
{
	return HealingItemInfo.ItemMesh;
}

bool UHealingItem::IsConsumable_Implementation() const
{
	return HealingItemInfo.bConsumable;
}

int32 UHealingItem::GetMaxStackCount_Implementation() const
{
	return HealingItemInfo.MaxStackCount;
}

float UHealingItem::GetHealingAmount() const
{
	return HealingItemInfo.HealingAmount;
}

void UHealingItem::SetHealingItemInfo(const FHealingItemInfo& HealingInfo)
{
	HealingItemInfo = HealingInfo;
}
