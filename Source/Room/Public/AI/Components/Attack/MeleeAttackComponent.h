// MeleeAttackComponent.h
// 근접 공격을 담당하는 AI 전용 컴포넌트
#pragma once

#include "CoreMinimal.h"
#include "BaseAttackComponent.h"
#include "MeleeAttackComponent.generated.h"

UCLASS()
class ROOM_API UMeleeAttackComponent : public UBaseAttackComponent
{
	GENERATED_BODY()
	
public:
	UMeleeAttackComponent();

	virtual void Attack() override;
};

