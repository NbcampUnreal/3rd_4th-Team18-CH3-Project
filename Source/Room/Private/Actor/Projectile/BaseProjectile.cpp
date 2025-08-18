// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/BaseProjectile.h"
#include "Actor/Character/BaseCharacter.h" 
#include "GameplayTagAssetInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/ObjectPoolSubsystem.h"
#include "UI/UISubsystem.h"

#include "Components/CapsuleComponent.h"

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

	SphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABaseProjectile::SetProjectileMoveData_Implementation(const FBulletItemData& BulletInfo)
{
	IProjectileDataReciever::SetProjectileMoveData_Implementation(BulletInfo);

	ProjectileMovement->InitialSpeed = BulletInfo.BulletInitialSpeed;
	ProjectileMovement->MaxSpeed = BulletInfo.BulletMaxSpeed;
	ReturnToPoolTime = BulletInfo.BulletAutoDestroyTime;
	
}

void ABaseProjectile::SetDamage_Implementation(float Damage)
{
	IProjectileDataReciever::SetDamage_Implementation(Damage);
	FinalDamage = Damage;
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnSphereOverlap);
	SphereComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnSphereHit);
}

void ABaseProjectile::Destroyed()
{
	Super::Destroyed();
	GetWorld()->GetTimerManager().ClearTimer(ReturnTimer);
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

//void ABaseProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
//	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	
//	if (!OtherActor || OtherActor == Shooter)
//		return;
//
//	// 피격 대상 체력 확인
//	if (ABaseCharacter* TargetChar = Cast<ABaseCharacter>(OtherActor))
//	{
//		if (TargetChar->HealthComponent && TargetChar->HealthComponent->GetCurrentHealth() <= 0.f)
//		{
//			// 체력 0이면 무시
//			return;
//		}
//	}
//	// 발사자 태그 가져오기
//	FGameplayTag OwnerTag;
//	if (ABaseCharacter* ShooterChar = Cast<ABaseCharacter>(Shooter))
//	{
//		FGameplayTagContainer OwnerTags;
//		ShooterChar->GetOwnedGameplayTags(OwnerTags); 
//		if (OwnerTags.Num() > 0)
//			OwnerTag = OwnerTags.GetByIndex(0);
//	}
//	// 피격 대상 태그 가져오기
//	FGameplayTag TargetTag;
//	if (ABaseCharacter* TargetChar = Cast<ABaseCharacter>(OtherActor))
//	{
//		FGameplayTagContainer TargetTags;
//		TargetChar->GetOwnedGameplayTags(TargetTags); 
//		if (TargetTags.Num() > 0)
//			TargetTag = TargetTags.GetByIndex(0);
//	}
//
//	// 태그가 다르면 데미지 적용
//	if (OwnerTag != TargetTag)
//	{
//		UGameplayStatics::ApplyDamage(OtherActor, FinalDamage, nullptr, Shooter, UDamageType::StaticClass());
//		UE_LOG(LogTemp, Warning, TEXT("Damage : %f"), FinalDamage);
//		if (GetWorld())
//			GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->ReturnPooledObject(this);
//	}
//}

/*
	2025-08-15 양한아
	[AI 관련 수정사항] : 데미지 처리 로직 수정
	- AI 캐릭터의 총알 감지용 스피어 컴포넌트와의 오버랩 시에는 데미지가 적용되지 않도록 설정
	- AI 캐릭터는 캡슐 컴포넌트나 스켈레탈 메시와 오버랩할 때만 데미지가 적용되도록 수정
	- 이로 인해, 총알이 AI 캐릭터의 스피어 컴포넌트에 닿아도 데미지를 주지 않음
*/
void ABaseProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == Shooter)
		return;

	//UE_LOG(LogTemp, Warning, TEXT("[BaseProjectile] OnSphereOverlap - Actor: %s, Component: %s"), *OtherActor->GetName(), *OtherComp->GetName());
	
	if (ABaseCharacter* TargetChar = Cast<ABaseCharacter>(OtherActor))
	{
		// 피격 대상 체력 확인
		if (TargetChar->HealthComponent && TargetChar->HealthComponent->GetCurrentHealth() <= 0.f)
			return;

		// BulletDetectionSphere인 경우: 감지만 하고 데미지는 주지 않음
		if (OtherComp->GetName().Contains(TEXT("BulletDetection")))
		{
			//UE_LOG(LogTemp, Warning, TEXT("[BaseProjectile] Hit BulletDetectionSphere - No damage, continue"));
			return; // 감지만 하고 총알은 계속 진행
		}

		// 실제 캐릭터 컴포넌트인 경우 : 데미지 적용
		if (OtherComp->IsA<UCapsuleComponent>() || OtherComp->IsA<USkeletalMeshComponent>())
		{
			//UE_LOG(LogTemp, Warning, TEXT("[BaseProjectile] Hit actual character component: %s"), *OtherComp->GetName());

			// 발사자 태그 가져오기
			FGameplayTag OwnerTag;
			if (ABaseCharacter* ShooterChar = Cast<ABaseCharacter>(Shooter))
			{
				FGameplayTagContainer OwnerTags;
				ShooterChar->GetOwnedGameplayTags(OwnerTags);
				if (OwnerTags.Num() > 0)
					OwnerTag = OwnerTags.GetByIndex(0);
			}

			// 피격 대상 태그 가져오기
			FGameplayTag TargetTag;
			FGameplayTagContainer TargetTags;
			TargetChar->GetOwnedGameplayTags(TargetTags);
			if (TargetTags.Num() > 0)
				TargetTag = TargetTags.GetByIndex(0);

			// 태그가 다르면 데미지 적용
			if (OwnerTag != TargetTag)
			{
				UGameplayStatics::ApplyDamage(OtherActor, FinalDamage, nullptr, Shooter, UDamageType::StaticClass());
				UE_LOG(LogTemp, Warning, TEXT("[BaseProjectile] Damage applied: %f to %s"), FinalDamage, *OtherActor->GetName());

				// 총알 제거
				if (GetWorld())
					GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->ReturnPooledObject(this);
			}
		}
	}
}

void ABaseProjectile::OnSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	//지형 충돌 시 풀
	if (OtherComp && (OtherComp->GetCollisionObjectType() == ECC_WorldStatic ||
				  OtherComp->GetCollisionObjectType() == ECC_WorldDynamic))
	{
		if (GetWorld())
			GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->ReturnPooledObject(this);
	}
}

