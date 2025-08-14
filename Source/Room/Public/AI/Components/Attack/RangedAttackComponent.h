// RangedAttackComponent.h
// 원거리 공격을 담당하는 AI 전용 컴포넌트
#pragma once

#include "CoreMinimal.h"
#include "BaseAttackComponent.h"
#include "Actor/Projectile/BaseProjectile.h"
#include "RangedAttackComponent.generated.h"

 class ABaseProjectile; // 총알 클래스

UCLASS()
class ROOM_API URangedAttackComponent : public UBaseAttackComponent
{
	GENERATED_BODY()
	
public:
	URangedAttackComponent();

	// AI의 전투 유형(Ranged)을 반환
	virtual EAICombatType GetCombatType() const override { return EAICombatType::Ranged; }

	// 공격 시작 처리 (애니메이션 재생, 투사체 발사 등)
	virtual void StartAttack() override;

	// 공격 종료 처리 (상태 초기화 등)
	virtual void StopAttack() override;

	// 총알 발사 (노티파이에서 호출)
	void PerformRangedAttack();

	// 설정된 발사체 클래스를 반환 (외부에서 스폰 시 참조 가능)
	TSubclassOf<AActor> GetProjectileClass() const { return ProjectileClass; }

protected:
	// 발사체 클래스 (화살, 총알 등)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	// 투사체가 생성될 소켓명 (총구 위치 기준)
	UPROPERTY(EditAnywhere, Category = "Ranged Attack")
	FName MuzzleSocketName = TEXT("Muzzle_01");
};
