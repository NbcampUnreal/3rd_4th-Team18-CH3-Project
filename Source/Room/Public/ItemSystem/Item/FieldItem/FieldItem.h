#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSystem/Interfaces/Interactable.h"
#include "FieldItem.generated.h"

class UItemBase;
UCLASS()
class ROOM_API AFieldItem : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AFieldItem();

protected:
	virtual void Interact_Implementation() override;
	virtual void InFocus_Implementation() override;
	virtual void OutFocus_Implementation() override;
	virtual FText GetInteractableMessage_Implementation() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Item")
	FDataTableRowHandle DataHandle;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Item")
	int32 Quantity = 1;
	UPROPERTY(BlueprintReadWrite, Category = "Item")
	TObjectPtr<UItemBase> Item;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<UMaterial> OutlineOverlayMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	FText InteractableMessage;
	
private:
	void Pickuped();
	
};

