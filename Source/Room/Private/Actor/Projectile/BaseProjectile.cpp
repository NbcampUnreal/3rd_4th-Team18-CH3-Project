// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/BaseProjectile.h"
#include "Actor/Character/BaseCharacter.h" 
#include "GameplayTagAssetInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/ObjectPoolSubsystem.h"
#include "UI/UISubsystem.h"

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
	if (!OtherActor || OtherActor == Shooter)
		return;

	// 발사자 태그 가져오기
	FGameplayTag OwnerTag;
	if (ABaseCharacter* ShooterChar = Cast<ABaseCharacter>(Shooter))
	{
		FGameplayTagContainer OwnerTags;
		ShooterChar->GetOwnedGameplayTags(OwnerTags); // 인터페이스 직접 호출
		if (OwnerTags.Num() > 0)
			OwnerTag = OwnerTags.GetByIndex(0);
	}
	// 피격 대상 태그 가져오기
	FGameplayTag TargetTag;
	if (ABaseCharacter* TargetChar = Cast<ABaseCharacter>(OtherActor))
	{
		FGameplayTagContainer TargetTags;
		TargetChar->GetOwnedGameplayTags(TargetTags); // 인터페이스 직접 호출
		if (TargetTags.Num() > 0)
			TargetTag = TargetTags.GetByIndex(0);
	}

	// 태그가 다르면 데미지 적용
	if (OwnerTag != TargetTag)
	{
		UGameplayStatics::ApplyDamage(OtherActor, FinalDamage, nullptr, Shooter, UDamageType::StaticClass());
		UE_LOG(LogTemp, Warning, TEXT("Damage : %f"), FinalDamage);
		if (GetWorld())
			GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->ReturnPooledObject(this);

		GetGameInstance()->GetSubsystem<UUISubsystem>()->ShowHitMarkerOnHUD();
	}
}

