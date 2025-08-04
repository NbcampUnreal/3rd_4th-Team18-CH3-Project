
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ItemSystem/Interfaces/Equipper.h"
#include "ItemPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ROOM_API AItemPlayerCharacter : public ACharacter, public IEquipper
{
	GENERATED_BODY()

public:
	AItemPlayerCharacter();
	virtual void Equip_Implementation(UObject* Equipment) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;
};
