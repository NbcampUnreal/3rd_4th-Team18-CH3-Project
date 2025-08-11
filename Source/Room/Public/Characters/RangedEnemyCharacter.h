// RangedEnemyCharacter.h
// 원거리 공격을 수행하는 AI 캐릭터 클래스
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI/Interface/BaseAICharacterInterface.h"		// AI 캐릭터 인터페이스 포함
#include "AI/Components/Attack/BaseAttackComponent.h"	// 기본 공격 컴포넌트 포함
#include "AI/Components/Attack/RangedAttackComponent.h"	// 원거리 공격 컴포넌트 포함
#include "RangedEnemyCharacter.generated.h"

UCLASS()
class ROOM_API ARangedEnemyCharacter : public ACharacter, public IBaseAICharacterInterface
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

	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHP = 100.0f;

	// 현재 체력
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float CurrentHP = 100.0f;

	// 사망 여부
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bIsDead = false;

	// 현재 장착된 공격 컴포넌트 반환 (원거리 공격 컴포넌트 캐스팅)
	virtual UBaseAttackComponent* GetAttackComponent() const override { return Cast<UBaseAttackComponent>(AttackComponent); }

	// 현재 AI 상태 반환
	//virtual EAIStateType GetCurrentState() const override { return CurrentState; }

	// AI 상태 설정
	//virtual void SetCurrentState(EAIStateType NewState) override { CurrentState = NewState; }

	// 걷기 속도 반환
	virtual float GetWalkSpeed() const override { return WalkSpeed; }

	// 달리기 속도 반환
	virtual float GetRunSpeed() const override { return RunSpeed; }

	// 현재 체력 반환
	virtual float GetCurrentHP() const override { return CurrentHP; }

	// 최대 체력 반환
	virtual float GetMaxHP() const override { return MaxHP; }

	// 사망 여부 반환
	virtual bool IsDead() const override { return bIsDead; }

	// 데미지 처리 함수 (데미지를 입었을 때 호출, DamageCauser는 데미지를 준 액터)
	virtual void TakeDamage(float DamageAmount, AActor* DamageCauser = nullptr) override;

	// 사망 처리 함수 (죽음 발생 시 호출, Killer는 죽인 액터)
	virtual void HandleDeath(AActor* Killer = nullptr) override;

	// 사망 애니메이션 재생 함수
	virtual void PlayDeathAnimation() override;

	// 이동 속도 변경 함수
	void SetMovementSpeed(float NewSpeed);

protected:
	// 원거리 공격 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	TObjectPtr<URangedAttackComponent> AttackComponent;

	// 현재 AI 상태 저장 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	EAIStateType CurrentState;

	virtual void BeginPlay() override;
};
