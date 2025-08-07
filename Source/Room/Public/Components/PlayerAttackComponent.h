#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Subsystem/StaticDataSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "PlayerAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOM_API UPlayerAttackComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UPlayerAttackComponent();

protected:
	virtual void BeginPlay() override;
	FTimerHandle FireTimerHandle;
public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentWeaponID = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Damage = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	float FireRate = 0.50f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Spread = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 BulletID = 0;
	//TODO: 무기 교체 기능 추가

	bool bIsFiring = false;
	void StartFire();
	void StopFire();
	void Fire();
};
