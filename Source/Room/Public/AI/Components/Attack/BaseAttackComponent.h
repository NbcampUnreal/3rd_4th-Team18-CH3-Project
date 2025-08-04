// BaseAttackComponent.h
// AI 캐릭터의 공격 컴포넌트의 기반(Base) 역할
/*
	목적 :  공격 로직을 캐릭터 본체가 아닌, 별도의 모듈(컴포넌트)로 분리함으로써 재사용성과 유지 보수성 향상.

	- SceneComponent를 상속받아 위치/방향 기반 공격이 필요한 경우를 지원
	- MeleeAttackComponent, RangedAttackComponent 등의 파생 컴포넌트가 이 클래스를 상속받아 공격 방식을 오버라이드할 수 있음.
*/
#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AI/Enums/AIEnums.h"
#include "BaseAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOM_API UBaseAttackComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UBaseAttackComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	EAICombatType CombatType;

public:
	UFUNCTION(BlueprintCallable)
	virtual void Attack();

	EAICombatType GetCombatType() const { return CombatType; }
};
