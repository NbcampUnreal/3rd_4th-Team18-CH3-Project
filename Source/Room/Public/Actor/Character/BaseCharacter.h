#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h" 
#include "Define/GameDefine.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "AI/Enums/AIEnums.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacter.generated.h"




UCLASS()
class ROOM_API ABaseCharacter : public ACharacter , public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	//상속 받은 캐릭터에서 태그 설정
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	UFUNCTION()
	virtual void HandleDeath();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UHealthComponent> HealthComponent;
    
	UFUNCTION(BlueprintCallable, Category = "States")
	FORCEINLINE bool IsDead() const { return bIsDead; }


	// 애니메이션 몽타주를 담을 TMap 변수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TMap<ECharacterAnim, TObjectPtr<UAnimMontage>> AnimMontages;

	// 현재 재생 중인 애니메이션 몽타주
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	ECharacterAnim CurrentAnimState;
    
	void RunMontage(ECharacterAnim Anim);
	bool StopMontage();    
	ECharacterAnim GetCurrentCharacterAnim() const;
    



protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool bIsDead = false;

	// 캐릭터가 소유한 게임플레이 태그를 저장하는 컨테이너
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer OwnedGameplayTags;
};