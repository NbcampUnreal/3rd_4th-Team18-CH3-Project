


#include "Core/GameManager.h"

#include "GameMapsSettings.h"
#include "Kismet/GameplayStatics.h"
#include "StaticData/RoomData.h"
#include "Subsystem/StaticDataSubsystem.h"
#include "Subsystem/LoadingSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameManager)

void UGameManager::Init()
{
	Super::Init();
	Config = UGameConfigData::Get();
}

void UGameManager::StartGame()
{
	Config = UGameConfigData::Get();
	// 전역 설정 데이터 에셋을 가져옵니다.
	if (!Config)
	{
		UE_LOG(LogTemp, Error, TEXT("UGameManager::StartGame - GameConfigData is not valid."));
		return;
	}

	// 시작 레벨 정보가 유효한지 확인합니다.
	if (Config->StartLevel.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("UGameManager::StartGame - StartLevel is not set in GameConfigData."));
		return;
	}

	// UStaticDataSubsystem* StaticData = GetSubsystem<UStaticDataSubsystem>();
	// ULoadingSubsystem* LoadingSubsystem = GetSubsystem<ULoadingSubsystem>();
	// if (StaticData && LoadingSubsystem)
	// {
	// 	// 0 번 키 = 첫번 째 레벨.
	// 	const FRoomData* RoomData = StaticData->GetData<FRoomData>(0);}
	
	// 로딩 서브시스템을 통해 로딩 화면과 함께 첫 레벨을 로드합니다.
	if (ULoadingSubsystem* LoadingSubsystem = GetSubsystem<ULoadingSubsystem>())
	{
		// 추가적으로 미리 로드할 리소스가 있다면 두 번째 인자에 배열로 전달할 수 있습니다.
		FString LevelName = Config->StartLevel.ToSoftObjectPath().GetAssetName();
		const FRoomData* RoomData = GetSubsystem<UStaticDataSubsystem>()->GetDataByKey<FRoomData, FName>(FName(LevelName));
		LoadingSubsystem->LoadLevelWithLoadingScreen(*RoomData);
	}
}

void UGameManager::ReturnToTitle()
{
	FString defaultMapName = GetDefault<UGameMapsSettings>()->GetGameDefaultMap();

	UGameplayStatics::OpenLevel(this, FName(defaultMapName));

	// TODO: 현재 로드해놨던 에셋들 언로드 필요.
	if (ULoadingSubsystem* LoadingSubsystem = GetSubsystem<ULoadingSubsystem>())
	{
		auto LoadedAsset = LoadingSubsystem->GetAllLoadedAssets();
		for (auto ObjPtr : LoadedAsset)
		{
			LoadingSubsystem->RequestUnload(ObjPtr);
		}
	}
}
