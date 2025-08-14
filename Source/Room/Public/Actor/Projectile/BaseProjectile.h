// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interface/PoolableInterface.h"
#include "ItemSystem/Interfaces/ProjectileDataReciever.h"
#include "BaseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class ROOM_API ABaseProjectile : public AActor, public IPoolableInterface, public IProjectileDataReciever
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();
	virtual void SetProjectileMoveData_Implementation(const FBulletItemData& BulletInfo) override;
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<AActor> Shooter;


	//~ Begin IPoolable Interface
	virtual void OnPoolBegin_Implementation(const FTransform& SpawnTransform) override;
	virtual void OnPoolEnd_Implementation() override;
	//~ End IPoolable Interface

	float ReturnToPoolTime = 5.0f;
private:
	float FinalDamage = 30.0f;
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
