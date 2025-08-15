// RangedEnemyCharacter.h
// 원거리 공격을 수행하는 AI 캐릭터 클래스
#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/BaseCharacter.h"
#include "GameFramework/Character.h"
#include "AI/Interface/BaseAICharacterInterface.h"		// AI 캐릭터 인터페이스 포함
#include "AI/Components/Attack/BaseAttackComponent.h"	// 기본 공격 컴포넌트 포함
#include "AI/Components/Attack/RangedAttackComponent.h"	// 원거리 공격 컴포넌트 포함
#include "RangedEnemyCharacter.generated.h"

class USphereComponent;

UCLASS()
class ROOM_API ARangedEnemyCharacter : public ABaseCharacter, public IBaseAICharacterInterface
{
	GENERATED_BODY()

public:
	ARangedEnemyCharacter();

	// AI 이동 속도 설정 (걷기 속도)
	UPROPERTY(EditAnywhere, Category = "AI")
	float WalkSpeed = 300.0f;

	// AI 이동 속도 설정 (달리기 속도)
	UPROPERTY(EditAnywhere, Category = "AI")
	float RunSpeed = 600.0f;

	// 목표 재생 시간
	UPROPERTY(EditAnywhere, Category = "AI")
	float DesiredDuration;

	// 이동 속도 변경 함수
	void SetMovementSpeed(float NewSpeed);

	// 걷기 속도 반환
	virtual float GetWalkSpeed() const override { return WalkSpeed; }

	// 달리기 속도 반환
	virtual float GetRunSpeed() const override { return RunSpeed; }

protected:
	// 원거리 공격 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	TObjectPtr<URangedAttackComponent> AttackComponent;

	// 총알 감지 범위로 사용할 스피어 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* BulletDetectionSphere;

	// 총알 감지 시 호출되는 함수
	UFUNCTION()
	void OnBulletDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// 발사자 추적 함수
	virtual void TrackBulletShooter(AActor* BulletShooter) override;

	virtual void BeginPlay() override;

	// 사망 처리 함수
	virtual void HandleDeath() override;
};
