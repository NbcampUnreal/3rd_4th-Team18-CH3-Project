#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

// 게임에서 필요하며 전역적으로 사용할 리터럴 값 정의장소
namespace GameDefine
{
	const FGameplayTag PlayerTag = FGameplayTag::RequestGameplayTag(TEXT("Actor.Player"));
	const FGameplayTag EnemyTag = FGameplayTag::RequestGameplayTag(TEXT("Actor.Enemy"));
}