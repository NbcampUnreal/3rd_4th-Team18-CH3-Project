#include "Subsystem/SoundSubsystem.h"

#include "Kismet/GameplayStatics.h"

#include "Core/RoomGameMode.h"
#include "Core/RoomGameState.h"

#include "Components/AudioComponent.h"
#include "Subsystem/StaticDataSubsystem.h"

#include "StaticData/SoundData.h"

void USoundSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FWorldDelegates::OnPostWorldCreation.AddUObject(this, &USoundSubsystem::OnWorldCreated);
}

void USoundSubsystem::Deinitialize()
{
	if (BGM_Component)
	{
		BGM_Component->DestroyComponent();
		BGM_Component = nullptr;
	}
	Super::Deinitialize();
}

void USoundSubsystem::OnWorldCreated(UWorld* World)
{
	if (!World || !World->GetAuthGameMode())
	{
		return;
	}

	ARoomGameMode* GameMode = Cast<ARoomGameMode>(World->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->OnStartRoom.AddDynamic(this, &USoundSubsystem::HandleRoomStart);
		ARoomGameState* GameState = World->GetGameState<ARoomGameState>();

		if (GameState && GameState->bIsRoomStarted)
		{
			HandleRoomStart();
		}
	}
}

void USoundSubsystem::HandleRoomStart()
{
	if (BGM_Component && BGM_Component->IsPlaying())
	{
		return;
	}

	PlayBGM(FName("DefaultBGM"));
}

void USoundSubsystem::PlayBGM(const FName& BGM_ID)
{
	UStaticDataSubsystem* StaticData = GetGameInstance()->GetSubsystem<UStaticDataSubsystem>();

	if (!StaticData)
	{
		return;
	}

	const FBGMData* BGMData = StaticData->GetDataByKey<FBGMData>(BGM_ID);

	if (!BGMData || !BGMData->Sound)
	{
		return;
	}

	if (BGM_Component && BGM_Component->IsPlaying())
	{
		StopBGM();
	}

	if (!BGM_Component)
	{
		BGM_Component = UGameplayStatics::CreateSound2D(GetWorld(), BGMData->Sound, 1.0f, 1.0f, 0.0f, nullptr, true);
	}

	if (BGM_Component)
	{
		BGM_Component->SetSound(BGMData->Sound);
		BGM_Component->FadeIn(BGMData->FadeInDuration);
	}
}

void USoundSubsystem::StopBGM()
{
	if (BGM_Component && BGM_Component->IsPlaying())
	{
		BGM_Component->FadeOut(1.0f, 0.0f);
	}
}

void USoundSubsystem::PlaySound2D(const FName& Sound_ID)
{
	UStaticDataSubsystem* StaticData = GetGameInstance()->GetSubsystem<UStaticDataSubsystem>();
	if (!StaticData)
	{
		return;
	}

	const FSoundData* SoundData = StaticData->GetDataByKey<FSoundData, FName>(Sound_ID);
	if (!SoundData || !SoundData->Sound)
	{
		return;
	}
}