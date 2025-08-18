#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/BaseCharacter.h"
#include "Input/InputConfig.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WeaponComponent.h"
#include "ItemSystem/InteractionComponent/InteractionComponent.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UUISubsystem;
UCLASS()
class ROOM_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FInputConfig InputConfig;

	UPROPERTY(EditDefaultsOnly, Category="Stat")
	UDataTable* PlayerStatDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float Speed = 700.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInteractionComponent> InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWeaponComponent> WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataID")
	int32 PlayerStatID = 2;

protected:

	virtual void HandleDeath() override;
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartFire();
	void StopFire();
	void Interact();
	void InventoryToggle();
	// ISpawnableFromStaticData interface
	virtual void InitializeFromStaticData(const FStaticData* InStaticData) override;



private:
	UFUNCTION()
	void OnPlayerHealthChanged(float CurrentHealth, float MaxHealth);
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UUISubsystem> UISubsystem;

};