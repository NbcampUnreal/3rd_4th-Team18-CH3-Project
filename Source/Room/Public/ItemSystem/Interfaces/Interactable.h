// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE()
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class ROOM_API IInteractable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Interactable")
	void Interact(AActor* Caller);

	UFUNCTION(BlueprintNativeEvent, Category = "Interactable")
	FText GetInteractableMessage();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interactable")
	void InFocus();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interactable")
	void OutFocus();
};
