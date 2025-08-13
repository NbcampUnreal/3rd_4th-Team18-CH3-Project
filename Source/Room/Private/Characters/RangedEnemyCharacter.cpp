// RangedEnemyCharacter.cpp
#include "Characters/RangedEnemyCharacter.h"
#include "AI/Components/Attack/RangedAttackComponent.h"
#include "AI/Controllers/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ARangedEnemyCharacter::ARangedEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// 공격 컴포넌트 생성 및 루트에 붙이기
	AttackComponent = CreateDefaultSubobject<URangedAttackComponent>(TEXT("RangedAttackComponent"));
	AttackComponent->SetupAttachment(RootComponent);

	// 초기 AI 상태
	CurrentState = EAIStateType::Idle;

	// AI 컨트롤러 지정 및 자동 빙의 설정
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 이동 관련 세팅
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // 회전 속도
	Movement->MaxWalkSpeed = WalkSpeed; // 기본 이동 속도
	Movement->AirControl = 0.2f;
}


void ARangedEnemyCharacter::HandleDeath()
{
	bIsDead = true;
	//AI 컨트롤러 비활성화
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
	{
		AIController->StopMovement();
		AIController->UnPossess();
	}
}

void ARangedEnemyCharacter::PlayDeathAnimation()
{
	if (DeathMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(DeathMontage);
		}
	}
}

void ARangedEnemyCharacter::SetMovementSpeed(float NewSpeed)
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = NewSpeed;
	}
}

void ARangedEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	OwnedGameplayTags.AddTag(GameDefine::EnemyTag);
	UE_LOG(LogTemp, Warning, TEXT("[AI] AI Character has been spawned"));
}
