#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Define/GameDefine.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "BaseCharacter.generated.h"

UCLASS()
class ROOM_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	//상속 받은 캐릭터에서 태그 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag")
	FGameplayTag ActorTag;
	FORCEINLINE FGameplayTag GetActorTag() const { return ActorTag; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UHealthComponent> HealthComponent;
	
	

	UFUNCTION(BlueprintCallable, Category = "States")
	FORCEINLINE bool IsDead() const { return bIsDead; }
	
	UFUNCTION()
	virtual void HandleDeath();

	
protected:
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool bIsDead = false;
};
