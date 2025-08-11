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
	
	//사망 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* DeathMontage;
	
	// 현재 캐릭터에 장착된 공격 컴포넌트 반환 (근접 공격 컴포넌트를 기본으로 캐스팅)
	virtual UBaseAttackComponent* GetAttackComponent() const override { return Cast<UBaseAttackComponent>(AttackComponent); }

	// 현재 AI 상태 반환
	//virtual EAIStateType GetCurrentState() const override { return CurrentState; }

	// AI 상태 설정
	//virtual void SetCurrentState(EAIStateType NewState) override { CurrentState = NewState; }

	// 걷기 속도 반환
	virtual float GetWalkSpeed() const override { return WalkSpeed; }

	// 달리기 속도 반환
	virtual float GetRunSpeed() const override { return RunSpeed; }
	
	// 사망 처리 함수 (죽인 액터 정보 포함 가능)
	virtual void HandleDeath() override;

	// 사망 시 재생할 애니메이션 함수
	virtual void PlayDeathAnimation() override;

	// 캐릭터 이동 속도를 변경하는 함수
	void SetMovementSpeed(float NewSpeed);

	
protected:
	// 근접 공격 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	TObjectPtr<UMeleeAttackComponent> AttackComponent;

	// 현재 AI 상태 저장 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	EAIStateType CurrentState;

	virtual void BeginPlay() override;
};
