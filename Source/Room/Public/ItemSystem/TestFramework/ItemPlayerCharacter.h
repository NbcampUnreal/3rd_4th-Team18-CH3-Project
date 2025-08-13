
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ItemPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ROOM_API AItemPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AItemPlayerCharacter();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;
};
