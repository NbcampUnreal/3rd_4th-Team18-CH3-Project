// MeleeAttackNotifyState.cpp
#include "AI/Components/Attack/Notifies/MeleeAttackNotifyState.h"
#include "AI/Components/Attack/MeleeAttackComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"

void UMeleeAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (!MeshComp)
		return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
		return;

	// 컴포넌트에서 MeleeAttackComponent 찾기
	UMeleeAttackComponent* MeleeAttackComp = Owner->FindComponentByClass<UMeleeAttackComponent>();
	if (MeleeAttackComp)
	{
		MeleeAttackComp->PerformMeleeAttack();
		//UE_LOG(LogTemp, Log, TEXT("[AI][MeleeAttackNotifyState] PerformMeleeAttack called on %s"), *Owner->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AI][MeleeAttackNotifyState] MeleeAttackComponent not found on %s"), *Owner->GetName());
	}
}
