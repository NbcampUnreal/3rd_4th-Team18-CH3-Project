// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class ROOM_API UItemBase : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	void Use(AActor* User);

	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	FName GetItemID() const;
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	FName GetItemName() const;
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	FText GetItemDescription() const;
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	UTexture2D* GetItemIcon() const;
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	UStaticMesh* GetItemMesh() const;
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	bool IsConsumable() const;
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	int32 GetMaxStackCount() const;
	
};
