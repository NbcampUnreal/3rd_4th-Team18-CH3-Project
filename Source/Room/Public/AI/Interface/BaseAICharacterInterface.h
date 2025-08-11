// BaseAICharacterInterface.h
// 이 인터페이스는 AI 캐릭터가 공통적으로 수행해야 하는 기능들을 정의.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AI/Enums/AIEnums.h" // 열거형 포함
#include "BaseAICharacterInterface.generated.h"

class UBaseAttackComponent;

UINTERFACE(MinimalAPI)
class UBaseAICharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class ROOM_API IBaseAICharacterInterface
{
	GENERATED_BODY()

public:
	virtual class UBaseAttackComponent* GetAttackComponent() const = 0;
	//virtual EAIStateType GetCurrentState() const = 0;
	//virtual void SetCurrentState(EAIStateType NewState) = 0;

	// 이동 속도 설정 함수
	virtual void SetMovementSpeed(float Speed) = 0;

	// 이동 속도 가져오기 함수들
	virtual float GetWalkSpeed() const = 0;
	virtual float GetRunSpeed() const = 0;

	// 체력 관련 함수들 추가
	virtual float GetCurrentHP() const = 0;
	virtual float GetMaxHP() const = 0;
	virtual void TakeDamage(float DamageAmount, AActor* DamageCauser = nullptr) = 0;
	virtual bool IsDead() const = 0;

	// 죽음 처리 함수들 추가
	virtual void HandleDeath(AActor* Killer = nullptr) = 0;
	virtual void PlayDeathAnimation() = 0;
};