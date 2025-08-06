// MeleeEnemyCharacter.h
// 근접 전투를 수행하는 AI 캐릭터 클래스
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI/Interface/BaseAICharacterInterface.h"
#include "AI/Components/Attack/BaseAttackComponent.h"
#include "AI/Components/Attack/MeleeAttackComponent.h"
#include "MeleeEnemyCharacter.generated.h"

UCLASS()
class ROOM_API AMeleeEnemyCharacter : public ACharacter, public IBaseAICharacterInterface
{
	GENERATED_BODY()

public:
	AMeleeEnemyCharacter();

	UPROPERTY(EditAnywhere, Category = "AI")
	float WalkSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float RunSpeed = 600.0f;

	virtual UBaseAttackComponent* GetAttackComponent() const override { return Cast<UBaseAttackComponent>(AttackComponent); }
	virtual EAIStateType GetCurrentState() const override { return CurrentState; }
	virtual void SetCurrentState(EAIStateType NewState) override { CurrentState = NewState; }

	virtual float GetWalkSpeed() const override { return WalkSpeed; }
	virtual float GetRunSpeed() const override { return RunSpeed; }

	void SetMovementSpeed(float NewSpeed);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	TObjectPtr<UMeleeAttackComponent> AttackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	EAIStateType CurrentState;

	virtual void BeginPlay() override;
};