#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

UCLASS()
class ROOM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowInventory();

};
