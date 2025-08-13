// AIEnums.h
// AI의 이동 속도, 전투 타입, 상태, 인식 수준 등을 구분하는 데 사용
#pragma once

#include "CoreMinimal.h"
#include "AIEnums.generated.h"

//--------------------------------------------------
// AI 캐릭터의 이동 속도를 구분하기 위한 열거형
// Behavior Tree 내에서 이동 속도 설정 등에 활용
//--------------------------------------------------
UENUM(BlueprintType)
enum class ECharacterMovementSpeedType : uint8
{
	Walk	UMETA(DisplayName = "Walk"),	// 걷기 속도 (일반적인 순찰 등 느린 이동에 사용)
	Run		UMETA(DisplayName = "Run")		// 뛰기 속도 (추적 등 빠른 이동에 사용)
};

//--------------------------------------------------
// AI 캐릭터의 전투 유형을 정의하는 열거형
// AI가 근거리(Me)*, 원거리(Ranged), 혼합형(Mixed) 공격 중 어떤 방식을 사용하는지 구분
//--------------------------------------------------
UENUM(BlueprintType)
enum class EAICombatType : uint8
{
	None		UMETA(DisplayName = "None"),	// 기본값/미설정 상태
	Melee		UMETA(DisplayName = "Melee"),	// 근접 공격 타입 (Melee)
	Ranged		UMETA(DisplayName = "Ranged"),	// 원거리 공격 타입 (Ranged)
	Mixed		UMETA(DisplayName = "Mixed")	// 혼합형 (근거리 + 원거리 모두 사용하는 타입)
};

//--------------------------------------------------
// AI 캐릭터의 행동 상태를 나타내는 열거형
// Idle, 순찰, 추적, 공격, 조사, 사망 등 상태 구분에 사용
// Behavior Tree 상태 전환 및 Blackboard 값으로 활용
//--------------------------------------------------
UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Idle		UMETA(DisplayName = "Idle"),		// 대기 상태 (아무 행동도 하지 않는 상태)
	Patrol		UMETA(DisplayName = "Patrol"),		// 순찰 상태 (지정된 경로를 따라 이동)
	Chase		UMETA(DisplayName = "Chase"),		// 추적 상태 (타겟을 발견하여 따라가는 중)
	Attack		UMETA(DisplayName = "Attack"),		// 공격 상태 (타겟에게 공격을 가하는 중)
	Investigate UMETA(DisplayName = "Investigate"),	// 조사 상태 (의심스러운 소리나 흔적을 조사하는 중)
	Hit			UMETA(DisplayName = "Hit"),			// 피격 상태 (데미지를 받았지만 죽지는 않음)
	Dead		UMETA(DisplayName = "Dead")			// 사망 상태 (죽은 상태, 행동 불가)
};

//--------------------------------------------------
// AI가 플레이어 또는 목표를 인식하는 정도를 나타내는 열거형
// 무지(Unaware)부터 경계(Alerted), 전투(Engaged)까지 단계별 인식 수준 표현
// 스텔스 게임이나 탐지 시스템에 적합
//--------------------------------------------------
UENUM(BlueprintType)
enum class EAIAwarenessLevel : uint8
{
	Unaware		UMETA(DisplayName = "Unaware"),		// 완전히 인식하지 못함
	Suspicious	UMETA(DisplayName = "Suspicious"),	// 의심 상태 (무언가 이상함을 감지)
	Alerted		UMETA(DisplayName = "Alerted"),		// 경계 상태 (명확한 징후 발견, 집중 감시)
	Engaged		UMETA(DisplayName = "Engaged")		// 전투 상태 (타겟과 교전 중)
};


//--------------------------------------------------
//캐릭터 애니메이션 몽타주 재생 상태를 나타내는 열거형
//--------------------------------------------------
UENUM(BlueprintType)
enum class ECharacterAnim : uint8
{
	GetHit,
	Attacking,
	Dead,
	Idle
};