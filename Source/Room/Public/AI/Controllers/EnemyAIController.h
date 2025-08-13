// EnemyAIController.h
// 비헤이비어 트리와 인식 시스템(AIPerception)을 통해 적 AI의 행동을 제어하는 AIController
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "AI/Enums/AIEnums.h"
#include "EnemyAIController.generated.h"

class UBehaviorTree;
class UBlackboardComponent;
class UStaticDataSubsystem;
class UBaseAttackComponent;
class UAISenseConfig_Sight; // 시야 감지 구성 클래스
class AActor;

UCLASS()
class ROOM_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	// 테스트용 더미 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAIStateType DummyState;

	// 블랙보드에 저장된 타겟 액터 반환
	AActor* GetTargetActor() const;

	// AI의 상태를 외부에서 변경할 때 사용하는 함수
	void SetAIState(EAIStateType NewState);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Pawn이 Possess 되었을 때 호출됨
	virtual void OnPossess(APawn* InPawn) override;

	// 컨트롤러가 Pawn을 잃었을 때 호출되는 후처리 함수
	virtual void OnUnPossess() override;

	// 감지된 타겟에 대한 정보가 업데이트되었을 때 호출됨
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// AI Perception Component (감지 시스템 전체)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	// 감지 구성 중 시야(Sight)에 대한 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	// 블랙보드 컴포넌트 (BT에서 키-값 저장/조회용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBlackboardComponent> BlackboardComp;

	// 비헤이비어 트리
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	// 블랙보드 키 값 정의
	static const FName BBKey_TargetActor;				// 현재 타겟 액터
	static const FName BBKey_TargetLastKnownLocation;	// 타겟 마지막 위치
	static const FName BBKey_AIState;					// AI의 현재 상태
	static const FName BBKey_CombatType;				// 근거리 / 원거리 타입
	static const FName BBKey_IsPlayerVisible;			// 시야 내에 플레이어 존재 여부

	// 디버그 시각화용 타이머 핸들
	FTimerHandle DebugDrawTimerHandle;

	// 디버그용 시야/감지 상태 표시 여부 - 토글용 변수 (원하면 사용)
	bool bDrawDebug = true;

private:
	// 캐릭터를 기반으로 CombatType(근거리/원거리)을 블랙보드에 설정
	void SetCombatTypeToBlackboard(ACharacter* EnemyCharacter);
};
