// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PoolableInterface.h"
#include "BaseProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class ROOM_API ABaseProjectile : public AActor, public IPoolableInterface
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	//~ Begin IPoolable Interface
	virtual void OnPoolBegin_Implementation(const FTransform& SpawnTransform) override;
	virtual void OnPoolEnd_Implementation() override;
	//~ End IPoolable Interface

private:
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};