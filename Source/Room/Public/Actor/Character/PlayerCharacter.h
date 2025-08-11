#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/BaseCharacter.h"
#include "Input/InputConfig.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PlayerAttackComponent.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ROOM_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FInputConfig InputConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	float Health = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	float Attack = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	float Defense = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPlayerAttackComponent> PlayerAttackComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* DeathMontage;
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void HandleDeath() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartFire();
	void StopFire();

	UFUNCTION()
	void PlayFireMontage();
};