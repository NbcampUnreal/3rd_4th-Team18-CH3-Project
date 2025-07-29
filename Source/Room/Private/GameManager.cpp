


#include "GameManager.h"
#include "GameInstance\StaticDataSubsystem.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GameManager)

void UGameManager::Init()
{
	Super::Init();
}

void UGameManager::StartGame()
{
	auto staticData = GetSubsystem<UStaticDataSubsystem>();

	// 로딩에셋 등록
	// 로딩 타이틀 시작
	//UGameplayStatics::OpenLevel(this, "LoadingMap");
	//FStreamableHandle
}

void UGameManager::ReturnToTitle()
{
}
