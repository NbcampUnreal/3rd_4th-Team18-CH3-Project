// BTTask_MaintainOptimalDistance.cpp
#include "AI/Tasks/BTTask_MaintainOptimalDistance.h"
#include "AI/Components/Attack/BaseAttackComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"

UBTTask_MaintainOptimalDistance::UBTTask_MaintainOptimalDistance()
{
    NodeName = TEXT("Maintain Optimal Distance");
}

EBTNodeResult::Type UBTTask_MaintainOptimalDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (!OwnerPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("[AI][MaintainOptimalDistance] No Pawn"));
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("[AI][MaintainOptimalDistance] No BlackboardComponent"));
        return EBTNodeResult::Failed;
    }

    //=====================================
    // AttackComponent에서 설정값 가져오기
    //=====================================
    UBaseAttackComponent* AttackComponent = OwnerPawn->FindComponentByClass<UBaseAttackComponent>();
    if (!AttackComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("[AI][MaintainOptimalDistance] No AttackComponent on %s"), *OwnerPawn->GetName());
        return EBTNodeResult::Failed;
    }

    //=====================================
    // 비율을 이용해 거리 계산
    //=====================================
    float AttackRange = AttackComponent->GetAttackData().AttackRange;   // 공격 반경 = 최적 거리
    float TooCloseDistance = AttackRange * TooCloseRatio;               // 예: 800 * 0.7 = 560
    float TooFarDistance = AttackRange * TooFarRatio;                   // 예: 800 * 1.3 = 1040

    //=====================================
    // 플레이어 위치 가져오기
    //=====================================
    FVector PlayerLocation = FVector::ZeroVector;
    AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));

    if (TargetActor)
    {
        PlayerLocation = TargetActor->GetActorLocation();
        //UE_LOG(LogTemp, Log, TEXT("[AI][MaintainDistance] Got player location from TargetActor: %s"), *PlayerLocation.ToString());
    }
    else
    {
        // TargetActor가 없으면 TargetLastKnownLocation 사용
        PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("TargetLastKnownLocation"));
        //UE_LOG(LogTemp, Warning, TEXT("[AI][MaintainDistance] Using TargetLastKnownLocation: %s"), *PlayerLocation.ToString());

        if (PlayerLocation.IsZero())
        {
            UE_LOG(LogTemp, Error, TEXT("[AI][MaintainDistance] No valid target location found!"));
            return EBTNodeResult::Failed;
        }
    }

    //=====================================
    // 거리 계산
    //=====================================
    FVector OwnerLocation = OwnerPawn->GetActorLocation();
    float CurrentDistance = FVector::Dist(OwnerLocation, PlayerLocation);

    //=====================================
    // 플레이어 방향 바라보기
    //=====================================
    FVector DirectionToPlayer = (PlayerLocation - OwnerLocation).GetSafeNormal();
    FRotator TargetRotation = DirectionToPlayer.Rotation();
    TargetRotation.Pitch = 0.0f;
    TargetRotation.Roll = 0.0f;

    float AngleDifference = FMath::Abs(FMath::FindDeltaAngleDegrees(OwnerPawn->GetActorRotation().Yaw, TargetRotation.Yaw));

    // 각도 차이가 10도 이상일 때만 회전
    if (AngleDifference > 10.0f)
    {
        if (TargetActor)
        {
            OwnerComp.GetAIOwner()->SetFocus(TargetActor);
        }
        else
        {
            OwnerComp.GetAIOwner()->SetFocalPoint(PlayerLocation);
        }

        //UE_LOG(LogTemp, Log, TEXT("[AI][MaintainDistance] %s focusing on player (Angle diff: %.1f)"),
        //    *OwnerPawn->GetName(), AngleDifference);
    }

    //=====================================
    // 거리에 따른 이동 결정
    //=====================================
    //UE_LOG(LogTemp, Warning, TEXT("[AI][MaintainDistance] DEBUG - %s: Current=%.1f, TooClose=%.1f, TooFar=%.1f"),
    //    *OwnerPawn->GetName(), CurrentDistance, TooCloseDistance, TooFarDistance);

    if (CurrentDistance < TooCloseDistance)
    {// 너무 가까움 -> 후진 (도망가기)
        FVector BackwardDirection = (OwnerLocation - PlayerLocation).GetSafeNormal();
        FVector TargetLocation = OwnerLocation + (BackwardDirection * MovementSpeed);

        //UE_LOG(LogTemp, Warning, TEXT("[AI][MaintainDistance] BACKING AWAY - From: %s To: %s"),
        //    *OwnerLocation.ToString(), *TargetLocation.ToString());

        OwnerComp.GetAIOwner()->MoveToLocation(TargetLocation, 5.0f);
        //UE_LOG(LogTemp, Warning, TEXT("[AI][MaintainDistance] MoveToLocation called for backing away"));
    }
    else
    {// 적정 거리 또는 멀음 -> 이동 정지 (멀면 어차피 Chase로 전환됨)
        OwnerComp.GetAIOwner()->StopMovement();
        //UE_LOG(LogTemp, Log, TEXT("[AI][MaintainDistance] %s staying in position (%.1f)"),
        //    *OwnerPawn->GetName(), CurrentDistance);
    }

    return EBTNodeResult::Succeeded;
}