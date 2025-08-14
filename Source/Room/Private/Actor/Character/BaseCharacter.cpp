#include "Actor/Character/BaseCharacter.h"
#include "StaticData/EnemyData.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/RoomGameMode.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HealthComponent)
	{
		HealthComponent->OnDead.AddDynamic(this, &ABaseCharacter::HandleDeath);
		HealthComponent->OnHit.AddDynamic(this, &ABaseCharacter::HandleHit);
	}

	if (UWorld* W = GetWorld())
	{
		W->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this]()
		{
			if (UWorld* W2 = GetWorld())
			{
				if (ARoomGameMode* GM = Cast<ARoomGameMode>(W2->GetAuthGameMode()))
				{
					GM->NotifyActorSpawn(this);
				}
			}
		}));
	}
}

void ABaseCharacter::HandleDeath()
{
	bIsDead = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->DisableMovement();   // 이동 정지
	}
	
	//컨트롤러 해제 등은 상속받은 캐릭터에서 처리
	ARoomGameMode* RoomGameMode = Cast<ARoomGameMode>( GetWorld()->GetAuthGameMode());
	if (RoomGameMode)
	{
		RoomGameMode->NotifyActorDead(this);
	}
}

void ABaseCharacter::HandleHit()
{
	RunMontage(ECharacterAnim::GetHit);
}

void ABaseCharacter::RunMontage(ECharacterAnim Anim)
{
	if (AnimMontages.Contains(Anim))
	{
		UAnimMontage* MontageToPlay = AnimMontages[Anim];

		// 애니메이션 몽타주가 유효 시
		if (MontageToPlay && GetMesh()->GetAnimInstance())
		{
			// 애니메이션 몽타주를 재생
			GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);

			// 현재 애니메이션 상태를 업데이트
			CurrentAnimState = Anim;
		}
	}
}

bool ABaseCharacter::StopMontage()
{
	if (GetMesh()->GetAnimInstance() && GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		// 모든 몽타주를 중지
		GetMesh()->GetAnimInstance()->Montage_Stop(0.2f);
        
		// 애니메이션 상태 초기화
		CurrentAnimState = ECharacterAnim::Idle; 

		return true;
	}
	return false;
}

ECharacterAnim ABaseCharacter::GetCurrentCharacterAnim() const
{
	return CurrentAnimState;
}

float ABaseCharacter::GetMontagePlayLength(ECharacterAnim AnimType) const
{
	if (const TObjectPtr<UAnimMontage>* MontagePtr = AnimMontages.Find(AnimType))
	{
		if (*MontagePtr)
		{
			return (*MontagePtr)->GetPlayLength();
		}
	}
	return 1.0f; // 기본 fallback 값
}

void ABaseCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(OwnedGameplayTags);
}

void ABaseCharacter::InitializeFromStaticData(const FStaticData* InStaticData)
{
	const FEnemyData* EnemyData = static_cast<const FEnemyData*>(InStaticData);
	if (!EnemyData)
	{
		UE_LOG(LogTemp, Error, TEXT("ABaseCharacter::InitializeFromStaticData: Failed to cast to FEnemyData."));
		return;
	}

	if (HealthComponent)
	{
		HealthComponent->SetMaxHealth(EnemyData->Stat.HP);
		//HealthComponent->SetCurrentHealth(EnemyData->Stat.HP); // ????
	}

	// Set other properties from EnemyData->Stat as needed (Attack, Defense, etc.)

	// AI Controller setup
	AAIController* AIController = GetController<AAIController>();
	if (AIController && EnemyData->Behavior)
	{
		AIController->RunBehaviorTree(EnemyData->Behavior.Get());
	}
}
