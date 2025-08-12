// BaseAttackComponent.h
// AI 캐릭터의 공격 컴포넌트의 기반(Base) 역할
/*
	목적 : 공격 로직을 캐릭터 본체가 아닌, 별도의 모듈(컴포넌트)로 분리함으로써 재사용성과 유지 보수성 향상.

	- SceneComponent를 상속받아 위치/방향 기반 공격이 필요한 경우를 지원
	- MeleeAttackComponent, RangedAttackComponent 등의 파생 컴포넌트가 이 클래스를 상속받아 공격 방식을 오버라이드할 수 있음.
*/
#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AI/Enums/AIEnums.h"
#include "BaseAttackComponent.generated.h"

// 공격 관련 정보 구조체
USTRUCT(BlueprintType)
struct FEnemyAttackData
{
	GENERATED_BODY()

	// 공격 유효 거리 (Enemy 기준으로 전방으로 얼마나 멀리 공격이 닿는지, ex: 검의 길이)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Enemy Attack Data")
	float AttackRange = 150.f;

	// 공격 반경 (전방 공격 범위의 넓이, ex: 좌우로 퍼지는 공격의 범위)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Enemy Attack Data")
	float AttackRadius = 150.f;

	// 공격이 대상에게 입히는 데미지 양
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Enemy Attack Data")
	float AttackDamage = 20.f;

	// 공격 후 다음 공격까지 대기해야 하는 시간 (초 단위, 쿨다운)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Enemy Attack Data")
	float AttackCooldown = 1.5f;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOM_API UBaseAttackComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UBaseAttackComponent();

	// 현재 공격 중인지 표시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Attack")
	bool bIsAttacking = false;

	// 공격이 가능한 상태인지 판단 (쿨타임 등)
	UFUNCTION(BlueprintCallable, Category = "AI Attack")
	virtual bool CanAttack() const;

	// 공격 시작 처리 (애니메이션 재생)
	UFUNCTION(BlueprintCallable, Category = "AI Attack")
	virtual void StartAttack();

	// 공격 종료 처리
	UFUNCTION(BlueprintCallable, Category = "AI Attack")
	virtual void StopAttack();

	// AI의 전투 유형을 반환
	virtual EAICombatType GetCombatType() const { return EAICombatType::None; }

	// 데이터 접근 함수
	FEnemyAttackData GetAttackData() const { return AttackData; }

protected:
	// 근접 / 원거리 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Attack")
	EAICombatType CombatType;

	// 공격 데이터 구조체 (범위, 쿨타임, 데미지 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Attack")
	FEnemyAttackData AttackData;

	// 마지막 공격 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Attack")
	float LastAttackTime = 0.0f;
};
