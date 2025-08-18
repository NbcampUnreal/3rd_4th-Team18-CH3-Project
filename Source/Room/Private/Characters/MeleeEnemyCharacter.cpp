// MeleeEnemyCharacter.cpp
#include "Characters/MeleeEnemyCharacter.h"
#include "AI/Components/Attack/MeleeAttackComponent.h"
#include "AI/Controllers/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/Projectile/BaseProjectile.h"	// 총알 클래스
#include "Components/SphereComponent.h"			// 스피어 컴포넌트
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

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

	// 목표 재생 시간
	DesiredDuration = 5.0f;

	//======================================================
	// 총알 감지 범위 설정
	//======================================================
	// 스피어 컴포넌트 생성 (총알 감지 범위)
	BulletDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("BulletDetectionSphere"));
	BulletDetectionSphere->SetupAttachment(RootComponent);
	BulletDetectionSphere->InitSphereRadius(500.f);	// 감지 반경 설정
	
	BulletDetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 충돌을 감지만 하고 실제 물리적인 충돌은 처리하지 않음
	BulletDetectionSphere->SetCollisionObjectType(ECC_Pawn);
	BulletDetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore); // 모든 채널 무시
	BulletDetectionSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap); // WorldDynamic 채널에 대해 오버랩

	// 오버랩 이벤트 바인딩
	BulletDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AMeleeEnemyCharacter::OnBulletDetected);

	// 디버그용 총알 감지 스피어 표시 여부 - 토글용 변수
	bShowBulletDetectionSphere = false; // 기본값은 상태 표시 안 함

	//======================================================
	// 캐릭터가 속한 태그 추가
	//======================================================
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

	if (BulletDetectionSphere)
	{
		BulletDetectionSphere->SetHiddenInGame(!bShowBulletDetectionSphere);
		BulletDetectionSphere->SetVisibility(bShowBulletDetectionSphere);
	}
	UE_LOG(LogTemp, Warning, TEXT("[AI][MeleeEnemyCharacter] AI Character has been spawned"));
}

void AMeleeEnemyCharacter::OnBulletDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OverlappedComponent != BulletDetectionSphere)
		return;

	if (ABaseProjectile* Projectile = Cast<ABaseProjectile>(OtherActor))
	{
		AActor* Shooter = Projectile->Shooter;
		if (Shooter && Shooter != this)
		{
			TrackBulletShooter(Shooter);
		}
	}
}

void AMeleeEnemyCharacter::TrackBulletShooter(AActor* BulletShooter)
{
	if (!BulletShooter)
	{
		UE_LOG(LogTemp, Error, TEXT("[AI][MeleeEnemyCharacter] BulletShooter is NULL!"));
		return;
	}

	FVector ShooterLocation = BulletShooter->GetActorLocation();

	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
	{

		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		if (BlackboardComp)
		{
			// Blackboard에 플레이어 정보 설정 - BT가 자동으로 추적할 것임
			BlackboardComp->SetValueAsObject(TEXT("TargetActor"), BulletShooter);
			BlackboardComp->SetValueAsBool(TEXT("IsPlayerVisible"), true);
			BlackboardComp->SetValueAsVector(TEXT("TargetLastKnownLocation"), ShooterLocation);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[AI][MeleeEnemyCharacter] Blackboard NOT FOUND!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AI][MeleeEnemyCharacter] AIController NOT FOUND!"));
	}
}
