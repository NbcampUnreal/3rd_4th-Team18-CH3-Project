// RangedAttackComponent.h
// 원거리 공격을 담당하는 AI 전용 컴포넌트
#pragma once

#include "CoreMinimal.h"
#include "BaseAttackComponent.h"
#include "RangedAttackComponent.generated.h"

UCLASS()
class ROOM_API URangedAttackComponent : public UBaseAttackComponent
{
	GENERATED_BODY()
	
public:
	URangedAttackComponent();

	virtual void Attack() override;
};
