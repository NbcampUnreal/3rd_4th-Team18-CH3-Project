// EnemyAIController.h
// 비헤이비어 트리와 인식 시스템을 통해 적의 행동을 제어하는 AI 컨트롤러
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
class UAISenseConfig_Sight;		// 시야 감지 설정
class AActor;

UCLASS()
class ROOM_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAIStateType DummyState;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBlackboardComponent> BlackboardComp;

	// 테스트용 기본 BT 등록용
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	FName BBKey_TargetActor = TEXT("TargetActor");
	FName BBKey_TargetLastKnownLocation = TEXT("TargetLastKnownLocation");
	FName BBKey_AIState = TEXT("AIState");
	FName BBKey_CombatType = TEXT("CombatType");

	FTimerHandle DebugDrawTimerHandle;
	bool bIsTargetDetected = false;

	void DrawDebugSight();
	
};
