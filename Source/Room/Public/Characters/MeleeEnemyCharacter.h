// MeleeEnemyCharacter.h
// 근접 전투를 수행하는 AI 캐릭터 클래스
#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/BaseCharacter.h"
#include "GameFramework/Character.h"
#include "AI/Interface/BaseAICharacterInterface.h"		// AI 캐릭터 인터페이스
#include "AI/Components/Attack/BaseAttackComponent.h"	// 기본 공격 컴포넌트
#include "AI/Components/Attack/MeleeAttackComponent.h"	// 근접 공격 컴포넌트
#include "MeleeEnemyCharacter.generated.h"

class USphereComponent;

UCLASS()
class ROOM_API AMeleeEnemyCharacter : public ABaseCharacter, public IBaseAICharacterInterface
{
	GENERATED_BODY()

public:
	AMeleeEnemyCharacter();

	// AI 이동 속도 설정 (걷기 속도)
	UPROPERTY(EditAnywhere, Category = "AI")
	float WalkSpeed = 300.0f;

	// AI 이동 속도 설정 (달리기 속도)
	UPROPERTY(EditAnywhere, Category = "AI")
	float RunSpeed = 600.0f;

	// 목표 재생 시간
	UPROPERTY(EditAnywhere, Category = "AI")
	float DesiredDuration;

	// 캐릭터 이동 속도를 변경하는 함수
	void SetMovementSpeed(float NewSpeed);
	
	// 걷기 속도 반환
	virtual float GetWalkSpeed() const override { return WalkSpeed; }

	// 달리기 속도 반환
	virtual float GetRunSpeed() const override { return RunSpeed; }

	// 디버그용 총알 감지 스피어 표시 여부 - 토글용 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Debug")
	bool bShowBulletDetectionSphere;
	
protected:
	// 근접 공격 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	TObjectPtr<UMeleeAttackComponent> AttackComponent;

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
