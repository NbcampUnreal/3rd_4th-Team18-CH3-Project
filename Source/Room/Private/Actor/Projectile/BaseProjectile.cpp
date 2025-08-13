// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/ObjectPoolSubsystem.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
}

void ABaseProjectile::SetProjectileMoveData_Implementation(const FBulletItemData& BulletInfo)
{
	IProjectileDataReciever::SetProjectileMoveData_Implementation(BulletInfo);

	ProjectileMovement->InitialSpeed = BulletInfo.BulletInitialSpeed;
	ProjectileMovement->MaxSpeed = BulletInfo.BulletMaxSpeed;
	ReturnToPoolTime = BulletInfo.BulletAutoDestroyTime;
	
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnSphereOverlap);
}

void ABaseProjectile::OnPoolBegin_Implementation(const FTransform& SpawnTransform)
{
	SetActorTransform(SpawnTransform);
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	 SphereComponent = GetComponentByClass<USphereComponent>();
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	ProjectileMovement->Activate();
	ProjectileMovement->Velocity = SpawnTransform.GetRotation().GetForwardVector() * ProjectileMovement->InitialSpeed;

	FTimerHandle ReturnTimer;
	GetWorldTimerManager().SetTimer(ReturnTimer, FTimerDelegate::CreateLambda([this]()
	{
		GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->ReturnPooledObject(this);
	}), ReturnToPoolTime, false);
}

void ABaseProjectile::OnPoolEnd_Implementation()
{
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SphereComponent = GetComponentByClass<USphereComponent>();
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ProjectileMovement->Deactivate();
	ProjectileMovement->Velocity = FVector::ZeroVector;
}

void ABaseProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->ReturnPooledObject(this);

	ACharacter* TargetChar = Cast<ACharacter>(OtherActor);
	if (TargetChar)
	{
		
		UGameplayStatics::ApplyDamage(TargetChar,FinalDamage,nullptr,nullptr,UDamageType::StaticClass());

	}
}
