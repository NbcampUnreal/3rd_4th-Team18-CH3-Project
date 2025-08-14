// MeleeEnemyCharacter.cpp
#include "Characters/MeleeEnemyCharacter.h"
#include "AI/Components/Attack/MeleeAttackComponent.h"
#include "AI/Controllers/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AMeleeEnemyCharacter::AMeleeEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// 공격 컴포넌트 생성 및 루트에 붙이기
	AttackComponent = CreateDefaultSubobject<UMeleeAttackComponent>(TEXT("MeleeAttackComponent"));
	AttackComponent->SetupAttachment(RootComponent);

	// AI 컨트롤러 지정 및 자동 빙의 설정
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 이동 관련 세팅
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // 회전 속도
	Movement->MaxWalkSpeed = WalkSpeed; // 기본 이동 속도
	Movement->AirControl = 0.2f;

	// 캐릭터가 속한 태그 추가
	OwnedGameplayTags.AddTag(GameDefine::EnemyTag);
}

void AMeleeEnemyCharacter::SetMovementSpeed(float NewSpeed)
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = NewSpeed;
	}
}

void AMeleeEnemyCharacter::HandleDeath()
{
	Super::HandleDeath();

	// AI 컨트롤러를 가져와서 Behavior Tree 중단
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
	{
		// Behavior Tree 중단
		AIController->StopBehaviorTree();
	}

	// 애니메이션 몽타주 재생 (죽는 애니메이션)
	/*
		문제점
		HealthComponent에서 SetLifeSpan(DestroyDelay)로 설정된 시간보다
		몽타주 재생 시간이 먼저 종료되면, 몽타주가 끝난 후에도
		SetLifeSpan(DestroyDelay)에 의해 캐릭터가 제거되기 전까지,
		죽는 애니메이션이 완료된 후
		ABP의 몽타주 외에 다른 Locomotion 애니메이션(예: Idle)이 재생되는 현상이 발생.

		해결 방법
		BaseCharacter의 RunMontage는 재생 시간을 늘릴 수 없으므로,
		직접 애니메이션 재생 함수를 사용하여 해결
	*/
	// Dead 애니메이션 몽타주 찾기
	UAnimMontage* DeadMontage = AnimMontages.FindRef(ECharacterAnim::Dead);

	// DeadMontage가 유효한지 확인하고 사용
	if (DeadMontage)
	{// 몽타주가 유효한 경우
		// 애니메이션 길이를 알고 있다고 가정하고 속도를 계산
		float AnimationLength = DeadMontage->GetPlayLength(); // 애니메이션 길이
		float DesiredDuration = 6.0f; // 목표 재생 시간

		// 속도 계산
		float Speed = AnimationLength / DesiredDuration;

		// 애니메이션 속도를 조정하여 목표 재생 시간 동안 재생
		GetMesh()->GetAnimInstance()->Montage_Play(DeadMontage, Speed);
	}
	else
	{// Dead 애니메이션 몽타주가 없을 경우
		UE_LOG(LogTemp, Warning, TEXT("[AI][MeleeEnemyCharacter] Dead Montage not found!"));
	}

	// AI 컨트롤러 비활성화
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
	{
		AIController->StopMovement();
		AIController->UnPossess();
	}
}

void AMeleeEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("[AI] AI Character has been spawned"));
}