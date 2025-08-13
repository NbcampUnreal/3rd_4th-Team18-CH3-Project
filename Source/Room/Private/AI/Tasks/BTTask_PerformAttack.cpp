// BTTask_PerformAttack.cpp
#include "AI/Tasks/BTTask_PerformAttack.h"
#include "AI/Components/Attack/BaseAttackComponent.h"
#include "AI/Components/Attack/MeleeAttackComponent.h"
#include "AI/Components/Attack/RangedAttackComponent.h"
#include "Characters/RangedEnemyCharacter.h"
#include "Characters/MeleeEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h"
#include "Engine/Engine.h" // UE_LOG를 쓰려면 포함

UBTTask_PerformAttack::UBTTask_PerformAttack()
{
    NodeName = TEXT("Perform Attack");
    bIsAttacking = false;
}

EBTNodeResult::Type UBTTask_PerformAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AIController로부터 Pawn 가져오기
    APawn* Pawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
    if (!Pawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("[AI][BTTask_PerformAttack] Failed: No Pawn from AIController."));
        return EBTNodeResult::Failed;
    }

    // 매 실행 시점에 Pawn에서 컴포넌트 가져오기
    /*
        Pawn이 바뀌거나 컴포넌트가 동적으로 붙는 경우에도 항상 최신 참조를 가져오기.
        호출 비용이 존재하겠지만, 공격 Task 같은 저빈도 호출에는 큰 영향이 없을 것으로 예상.

        멤버 변수로 컴포넌트 캐싱 (예: 클래스에 UBaseAttackComponent*를 미리 멤버로 둠)
        AI Pawn은 재사용되거나 교체될 수 있기 때문에, 캐싱한 컴포넌트 포인터가 오래되거나 nullptr가 될 수 있음.
        여러 Pawn에게 공유될 수 있어, 완전히 잘못된 참조로 이어질 수 있음.

        TWeakObjectPtr 등을 사용해서 일시적 캐싱.
        이 경우도 BTTask의 재사용성과 동시성 문제로 인해 불완전.
    */
    UBaseAttackComponent* AttackComponent = Pawn->FindComponentByClass<UBaseAttackComponent>();
    UMeleeAttackComponent* MeleeAttackComponent = Pawn->FindComponentByClass<UMeleeAttackComponent>();
    URangedAttackComponent* RangedAttackComponent = Pawn->FindComponentByClass<URangedAttackComponent>();

    // 공격 가능 상태인지 확인 (쿨타임 or 이미 공격 중이면 실패)
    if (!AttackComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("[AI][BTTask_PerformAttack] Failed: No AttackComponent on %s"), *Pawn->GetName());
        return EBTNodeResult::Failed;
    }

    //UE_LOG(LogTemp, Log, TEXT("[AI][BTTask_PerformAttack][%s] ExecuteTask called, bIsAttacking=%d"), *Pawn->GetName(), AttackComponent->bIsAttacking);

    // 공격 가능 상태인지 확인 (쿨타임 or 이미 공격 중이면 실패)
    if (!AttackComponent->CanAttack())
    {
        //UE_LOG(LogTemp, Log, TEXT("[AI][BTTask_PerformAttack][%s] Cannot attack yet (cooldown or attacking)."), *Pawn->GetName());
        return EBTNodeResult::Failed;
    }

    // 기본 공격 시간 (Fallback)
    float AttackDuration = 1.0f;

    if (MeleeAttackComponent && MeleeAttackComponent->CanAttack())
    {// 근접 공격이 가능할 경우 실행
        MeleeAttackComponent->StartAttack();

        if (AMeleeEnemyCharacter* MeleeCharacter = Cast<AMeleeEnemyCharacter>(Pawn))
        {
            AttackDuration = MeleeCharacter->GetMontagePlayLength(ECharacterAnim::Attacking);
        }
        //UE_LOG(LogTemp, Log, TEXT("[AI][BTTask_PerformAttack] Melee Attack started on %s (%.2fs)"), *Pawn->GetName(), AttackDuration);
    }
    else if (RangedAttackComponent && RangedAttackComponent->CanAttack())
    {// 원거리 공격이 가능할 경우 실행
        RangedAttackComponent->StartAttack();

        if (ARangedEnemyCharacter* RangedCharacter = Cast<ARangedEnemyCharacter>(Pawn))
        {
            AttackDuration = RangedCharacter->GetMontagePlayLength(ECharacterAnim::Attacking);
        }
        //UE_LOG(LogTemp, Log, TEXT("[AI][BTTask_PerformAttack] Ranged Attack started on %s (%.2fs)"), *Pawn->GetName(), AttackDuration);
    }
    else
    {// 기본 공격 컴포넌트 사용
        AttackComponent->StartAttack();
        AttackDuration = 1.0f;
        //UE_LOG(LogTemp, Log, TEXT("[AI][BTTask_PerformAttack] Base Attack started on %s"), *Pawn->GetName());
    }

    // 각 Pawn별로 개별 타이머 핸들 사용
    FTimerHandle& CurrentTimerHandle = PawnTimerHandles.FindOrAdd(TWeakObjectPtr<APawn>(Pawn));

    // 공격 종료 후 OnAttackFinished 콜백 실행
    FTimerDelegate TimerDel;
    TimerDel.BindUObject(this, &UBTTask_PerformAttack::OnAttackFinished, &OwnerComp);
    OwnerComp.GetWorld()->GetTimerManager().SetTimer(CurrentTimerHandle, TimerDel, AttackDuration, false);

    // 공격 완료 후 반환
    return EBTNodeResult::InProgress;
}

void UBTTask_PerformAttack::OnAttackFinished(UBehaviorTreeComponent* OwnerComp)
{
    if (!OwnerComp) return;

    APawn* Pawn = OwnerComp->GetAIOwner() ? OwnerComp->GetAIOwner()->GetPawn() : nullptr;
    if (!Pawn) return;

    UBaseAttackComponent* AttackComponent = Pawn->FindComponentByClass<UBaseAttackComponent>();
    UMeleeAttackComponent* MeleeAttackComponent = Pawn->FindComponentByClass<UMeleeAttackComponent>();
    URangedAttackComponent* RangedAttackComponent = Pawn->FindComponentByClass<URangedAttackComponent>();

    if (AttackComponent)
    {
        //UE_LOG(LogTemp, Log, TEXT("[AI][BTTask_PerformAttack][%s] Attack finished, stopping attack."), *Pawn->GetName());
        AttackComponent->bIsAttacking = false;
        AttackComponent->StopAttack();
    }

    if (MeleeAttackComponent) MeleeAttackComponent->StopAttack();
    if (RangedAttackComponent) RangedAttackComponent->StopAttack();

    // 맵에서 타이머 핸들 제거
    TWeakObjectPtr<APawn> PawnPtr(Pawn);
    PawnTimerHandles.Remove(PawnPtr);

    FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}

void UBTTask_PerformAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    APawn* Pawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
    if (!Pawn) return;

    UBaseAttackComponent* AttackComponent = Pawn->FindComponentByClass<UBaseAttackComponent>();

    if (AttackComponent)
    {
        //UE_LOG(LogTemp, Log, TEXT("[AI][BTTask_PerformAttack] Task finished while attacking, stopping attack."));
        AttackComponent->bIsAttacking = false;
        AttackComponent->StopAttack();
    }

    // 개별 타이머 핸들 정리
    TWeakObjectPtr<APawn> PawnPtr(Pawn);
    if (FTimerHandle* TimerHandle = PawnTimerHandles.Find(PawnPtr))
    {
        OwnerComp.GetWorld()->GetTimerManager().ClearTimer(*TimerHandle);
        PawnTimerHandles.Remove(PawnPtr);  // 맵에서 제거
        //UE_LOG(LogTemp, Log, TEXT("[AI][BTTask_PerformAttack] Timer cleared for %s"), *Pawn->GetName());
    }
}