// RangedAttackNotifyState.cpp
#include "AI/Components/Attack/Notifies/RangedAttackNotifyState.h"
#include "GameFramework/Actor.h"
#include "AI/Components/Attack/RangedAttackComponent.h"
#include "Components/SkeletalMeshComponent.h"

void URangedAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (!MeshComp)
        return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner)
        return;

    // 원거리 공격 컴포넌트를 찾아서 PerformRangedAttack 호출
    URangedAttackComponent* RangedAttackComp = Owner->FindComponentByClass<URangedAttackComponent>();
    if (RangedAttackComp)
    {
        RangedAttackComp->PerformRangedAttack();
        //UE_LOG(LogTemp, Log, TEXT("[AI][RangedAttackNotifyState] PerformRangedAttack called on %s"), *Owner->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[AI][RangedAttackNotifyState] RangedAttackComponent not found on %s"), *Owner->GetName());
    }
}
