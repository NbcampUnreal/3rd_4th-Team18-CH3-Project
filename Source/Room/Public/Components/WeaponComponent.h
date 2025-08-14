#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponComponent.generated.h"


class UInventoryComponent;
class UWeaponItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROOM_API UWeaponComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();
	void Equip(UWeaponItem* NewItem);
	void UnEquip();
	void Fire();
protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UWeaponItem> Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsPlayerWeaponComp = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AActor> ProjectileClass;

	TWeakObjectPtr<UInventoryComponent> InventoryRef;
};
