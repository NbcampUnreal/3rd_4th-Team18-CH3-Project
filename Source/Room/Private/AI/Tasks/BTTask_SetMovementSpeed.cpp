// BTTask_SetMovementSpeed.cpp
#include "AI/Tasks/BTTask_SetMovementSpeed.h"
#include "AIController.h"
//#include "AI/TmpCharacters/TmpAICharacter.h" // AI 캐릭터 헤더 포함
#include "AI/Interface/BaseAICharacterInterface.h"

UBTTask_SetMovementSpeed::UBTTask_SetMovementSpeed()
{
	NodeName = TEXT("Set Movement Speed");
}

EBTNodeResult::Type UBTTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// AI 컨트롤러 가져오기
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed; // AI 컨트롤러가 없으면 실패 처리

	// AI가 조종하는 Pawn(캐릭터) 가져오기
	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed; // Pawn이 없으면 실패 처리

	// 인터페이스 기반으로 캐스팅 시도
	if (!Pawn->Implements<UBaseAICharacterInterface>())
	{
		UE_LOG(LogTemp, Warning, TEXT("[AI][SetMovementSpeed] Pawn does not implement BaseAICharacterInterface"));
		return EBTNodeResult::Failed;
	}

	// Pawn을 AI 전용 캐릭터 클래스로 캐스팅 시도
	IBaseAICharacterInterface* AICharacter = Cast<IBaseAICharacterInterface>(Pawn);
	if (!AICharacter) return EBTNodeResult::Failed; // 캐스팅 실패 시 실패 처리

	// 이동 속도 변수 초기화
	float NewSpeed = 0.f;

	// SpeedType enum 값에 따라 캐릭터의 속도 변수에서 실제 속도 가져오기
	switch (SpeedType)
	{
	case ECharacterMovementSpeedType::Walk:
		NewSpeed = AICharacter->GetWalkSpeed(); // 걷기 속도 적용
		break;

	case ECharacterMovementSpeedType::Run:
		NewSpeed = AICharacter->GetRunSpeed(); // 뛰기 속도 적용
		break;

	default:
		return EBTNodeResult::Failed; // 정의되지 않은 값은 실패 처리
	}

	// 캐릭터의 이동 속도 변경 함수 호출
	AICharacter->SetMovementSpeed(NewSpeed);

	// 작업 성공 반환
	return EBTNodeResult::Succeeded;
}
