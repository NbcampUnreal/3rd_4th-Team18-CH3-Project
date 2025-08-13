// BTTask_PlayHitMontage.cpp
#include "AI/Tasks/BTTask_PlayHitMontage.h"
#include "AIController.h"
#include "Characters/MeleeEnemyCharacter.h"
#include "Characters/RangedEnemyCharacter.h"

UBTTask_PlayHitMontage::UBTTask_PlayHitMontage()
{
	NodeName = TEXT("Play Hit Montage");
}

EBTNodeResult::Type UBTTask_PlayHitMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("[AI][BTTask_PlayHitMontage] ExecuteTask started"));

	// AIController 가져오기
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		UE_LOG(LogTemp, Error, TEXT("[AI][BTTask_PlayHitMontage] Failed to get AIController."));
		return EBTNodeResult::Failed;
	}
	//UE_LOG(LogTemp, Warning, TEXT("[AI][BTTask_PlayHitMontage] AIController found"));

	// AI가 조종 중인 Pawn 가져오기
	APawn* Pawn = Controller->GetPawn();
	if (!Pawn)
	{
		UE_LOG(LogTemp, Error, TEXT("[AI][BTTask_PlayHitMontage] Failed to get Pawn."));
		return EBTNodeResult::Failed;
	}
	//UE_LOG(LogTemp, Warning, TEXT("[AI][BTTask_PlayHitMontage] Pawn found: %s"), *Pawn->GetName());

	// Pawn을 ABaseCharacter로 캐스팅 시도
	if (ABaseCharacter* AICharacter = Cast<ABaseCharacter>(Pawn))
	{
		// 피격 몽타주 실행
		AICharacter->RunMontage(ECharacterAnim::GetHit);
		return EBTNodeResult::Succeeded;
	}

	// 캐스팅 실패 시 로그 출력
	UE_LOG(LogTemp, Error, TEXT("[AI][BTTask_PlayHitMontage] Failed to cast to ABaseCharacter."));

	return EBTNodeResult::Failed;
}
