#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathDelegate);
class ACharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOM_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	void SetMaxHealth(float NewMaxHealth);
	FORCEINLINE float GetMaxHealth() const {return MaxHealth; }

	FDeathDelegate OnDead;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<ACharacter> CharacterRef = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Health")
	float CurrentHealth = 100.0;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Health")
	float DestroyDelay = 5.0f;
	
private:
	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	void SetCurrentHealth(float NewHealth);

};
