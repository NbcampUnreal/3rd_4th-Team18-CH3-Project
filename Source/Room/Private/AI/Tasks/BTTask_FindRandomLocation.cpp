// BTTask_FindRandomLocation.cpp
#include "AI/Tasks/BTTask_FindRandomLocation.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	// 이 BTTask 노드의 이름을 "Find Random Location"으로 설정.
	// 이 이름은 에디터의 Behavior Tree에서 이 노드를 구분하는 데 사용된다.
	NodeName = TEXT("Find Random Location");

	// LocationKey에 Vector 타입 필터를 추가.
	// Blackboard에서 이 키는 Vector 값(예: 위치 좌표)만 저장할 수 있게 된다.
	// GET_MEMBER_NAME_CHECKED는 컴파일 타임에 안전하게 LocationKey의 이름을 문자열로 추출하는 매크로.
	LocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomLocation, LocationKey));
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 1단계: 필요한 정보(제어할 AI, 폰, 내비게이션 시스템) 가져오기
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed; // AI 컨트롤러 없으면 실패 반환

	APawn* MyPawn = AIController->GetPawn();
	if (!MyPawn) return EBTNodeResult::Failed;		// 제어 중인 Pawn 없으면 실패 반환

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem) return EBTNodeResult::Failed;	// 내비게이션 시스템 없으면 실패 반환

	// 2단계: 랜덤 위치 찾기
	FNavLocation RandomLocation;
	bool bFound = NavSystem->GetRandomReachablePointInRadius(
		MyPawn->GetActorLocation(),	// 현재 위치를 중심으로
		SearchRadius,				// 설정된 반경 내에서
		RandomLocation				// 찾은 위치를 저장
	);

	// 3단계: 랜덤 위치를 찾았으면 블랙보드에 저장
	if (bFound)
	{
		// AI 컨트롤러에서 블랙보드 컴포넌트를 가져옴
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		if (BlackboardComp)
		{
			// 블랙보드에 찾은 랜덤 위치 벡터 값을 저장 (LocationKey는 블랙보드 키)
			BlackboardComp->SetValueAsVector(LocationKey.SelectedKeyName, RandomLocation.Location);
			//UE_LOG(LogTemp, Log, TEXT("[AI][FindRandom] Found new destination at: %s"), *RandomLocation.Location.ToString());
			return EBTNodeResult::Succeeded; // 작업 성공 반환
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[AI][FindRandom] Failed to find a destination"));
	// 찾기에 실패하거나 블랙보드가 없으면 실패 반환
	return EBTNodeResult::Failed;
}
