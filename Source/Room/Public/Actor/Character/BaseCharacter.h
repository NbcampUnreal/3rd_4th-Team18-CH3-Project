#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Define/GameDefine.h"
#include "Components/HealthComponent.h"
#include "BaseCharacter.generated.h"
//캐릭터, 에너미 공통 기능을 포함한 베이스
UCLASS()
class ROOM_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	//상속 받은 캐릭터에서 태그 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag")
	FGameplayTag ActorTag;
	FGameplayTag GetActorTag() const { return ActorTag; }
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void HandleDeath();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UHealthComponent> HealthComponent;


};
