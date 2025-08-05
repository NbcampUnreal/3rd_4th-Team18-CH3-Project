// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Subsystem/ObjectPoolSubsystem.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnComponentHit);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseProjectile::OnPoolBegin_Implementation(const FTransform& SpawnTransform)
{
	SetActorTransform(SpawnTransform);
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	USphereComponent* SphereComponent = GetComponentByClass<USphereComponent>();
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	ProjectileMovement->Activate();
	ProjectileMovement->Velocity = SpawnTransform.GetRotation().GetForwardVector() * ProjectileMovement->InitialSpeed;

	FTimerHandle ReturnTimer;
	GetWorldTimerManager().SetTimer(ReturnTimer, FTimerDelegate::CreateLambda([this]()
	{
		GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->ReturnPooledObject(this);
	}), 5.f, false);
}

void ABaseProjectile::OnPoolEnd_Implementation()
{
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	USphereComponent* SphereComponent = GetComponentByClass<USphereComponent>();
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ProjectileMovement->Deactivate();
	ProjectileMovement->Velocity = FVector::ZeroVector;
}

void ABaseProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 월드 이벤트 호출
	GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->ReturnPooledObject(this);
}