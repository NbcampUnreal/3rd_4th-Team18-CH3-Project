#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Define/GameDefine.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "ItemSystem/Interfaces/Equipper.h"
#include "BaseCharacter.generated.h"

UENUM()
enum class ECharacterAnim
{
	GetHit,
	Attacking,
	Dead,
	Idle
};

UCLASS()
class ROOM_API ABaseCharacter : public ACharacter, public IEquipper
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	//상속 받은 캐릭터에서 태그 설정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tag")
	FGameplayTag ActorTag;
	FORCEINLINE FGameplayTag GetActorTag() const { return ActorTag; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UHealthComponent> HealthComponent;
	
	
	UFUNCTION(BlueprintCallable, Category = "States")
	FORCEINLINE bool IsDead() const { return bIsDead; }
	
	UFUNCTION()
	virtual void HandleDeath();

	// 애니메이션 몽타주를 담을 TMap 변수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TMap<ECharacterAnim, TObjectPtr<UAnimMontage>> AnimMontages;

	// 현재 재생 중인 애니메이션 몽타주
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	ECharacterAnim CurrentAnimState;
	
	void RunMontage(ECharacterAnim Anim, float delay, float duration, float speed);
	bool StopMontage();	
	ECharacterAnim GetCurrentCharacterAnim() const;

protected:
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool bIsDead = false;
};
