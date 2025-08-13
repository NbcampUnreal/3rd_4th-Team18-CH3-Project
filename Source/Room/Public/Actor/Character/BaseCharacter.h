#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h" 
#include "Define/GameDefine.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "ItemSystem/Interfaces/Equipper.h"
#include "AI/Enums/AIEnums.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacter.generated.h"




UCLASS()
class ROOM_API ABaseCharacter : public ACharacter, public IEquipper, public IGameplayTagAssetInterface
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
    

	/*
		[NOTE]
		해당 클래스는 캐릭터 담당 영역이지만,
		캐릭터 담당자와 협의하여 AI 담당자가 이 함수를 추가했습니다.
		AI 로직에서 몽타주 재생 시간을 얻기 위해 사용됩니다.

		[TODO] 추후 캐릭터 담당자가 이 함수 리팩터링 또는 위치 변경할 수 있습니다.
	*/
	UFUNCTION(BlueprintCallable, Category = "Animation")
	float GetMontagePlayLength(ECharacterAnim AnimType) const;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool bIsDead = false;

	// 캐릭터가 소유한 게임플레이 태그를 저장하는 컨테이너
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer OwnedGameplayTags;
};