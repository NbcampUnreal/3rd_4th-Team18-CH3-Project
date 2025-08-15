// BaseAICharacterInterface.h
// 이 인터페이스는 AI 캐릭터가 공통적으로 수행해야 하는 기능들을 정의.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AI/Enums/AIEnums.h" // 열거형 포함
#include "BaseAICharacterInterface.generated.h"

class UBaseAttackComponent;

UINTERFACE(MinimalAPI)
class UBaseAICharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class ROOM_API IBaseAICharacterInterface
{
	GENERATED_BODY()

public:
	/*
		BTTask_SetMovementSpeed 관련
	*/
	// 이동 속도 설정 함수
	virtual void SetMovementSpeed(float Speed) = 0;

	// 이동 속도 가져오기 함수들
	virtual float GetWalkSpeed() const = 0;
	virtual float GetRunSpeed() const = 0;

	/*
		총알 감지 및 발사자 추적 관련 함수들
	*/
	// 총알 감지 함수 (총알이 들어오면 호출됨)
	virtual void OnBulletDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) = 0;

	// 총알의 발사자 추적 함수 (총알의 발사자 위치를 추적)
	virtual void TrackBulletShooter(AActor* BulletShooter) = 0;
};