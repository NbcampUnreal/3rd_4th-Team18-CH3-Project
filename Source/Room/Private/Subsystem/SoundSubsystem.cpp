#include "Subsystem/SoundSubsystem.h"
#include "StaticData/SoundData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

void USoundSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const FString SoundDataTablePath = TEXT("/Game/Data/DT_SoundDataTable.DT_SoundDataTable");
	SoundDataTable = LoadObject<UDataTable>(nullptr, *SoundDataTablePath);

	if (!SoundDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("DT_SoundDataTable not found at path: %s"), *SoundDataTablePath);
	}
}

void USoundSubsystem::Deinitialize()
{
	if (BGMComponent)
	{
		BGMComponent->Stop();
		BGMComponent->DestroyComponent();
		BGMComponent = nullptr;
	}

	Super::Deinitialize();
}

const FSoundData* USoundSubsystem::GetSoundData(FName SoundID) const
{
	if (!SoundDataTable) return nullptr;
	return SoundDataTable->FindRow<FSoundData>(SoundID, TEXT("ID"));
}

void USoundSubsystem::PlayBGM(FName SoundID, float FadeInDuration)
{
	if (SoundID == CurrentBGM_ID && BGMComponent && BGMComponent->IsPlaying())
	{
		return;
	}

	const FSoundData* SoundData = GetSoundData(SoundID);
	if (!SoundData || SoundData->Sound.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayBGM: invalid SoundID: %s"), *SoundID.ToString());
		return;
	}

	CurrentBGM_ID = SoundID;

	FStreamableManager& Streamable = UAssetManager::Get().GetStreamableManager();
	Streamable.RequestAsyncLoad(SoundData->Sound.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(
			this, &USoundSubsystem::OnBGM_Loaded, SoundData, FadeInDuration));
}

void USoundSubsystem::OnBGM_Loaded(const FSoundData* SoundData, float FadeInDuration)
{
	if (BGMComponent)
	{
		BGMComponent->Stop();
		BGMComponent->DestroyComponent();
		BGMComponent = nullptr;
	}

	BGMComponent = UGameplayStatics::CreateSound2D(
		GetWorld(), SoundData->Sound.Get(), SoundData->DefaultVolume, SoundData->DefaultPitch, 0.0f, nullptr, true, false); 

	if (BGMComponent)
	{
		BGMComponent->bIsUISound = true;
		BGMComponent->FadeIn(FadeInDuration, SoundData->DefaultVolume);
	}
}

void USoundSubsystem::StopBGM(float FadeOutDuration)
{
	if (BGMComponent && BGMComponent->IsPlaying())
	{
		BGMComponent->FadeOut(FadeOutDuration, 0.0f);
		CurrentBGM_ID = NAME_None;
	}
}

void USoundSubsystem::PlaySound2D(FName SoundID, float VolumeMultiplier, float PitchMultiplier)
{
	const FSoundData* SoundData = GetSoundData(SoundID);
	if (!SoundData || SoundData->Sound.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("PlaySound2D: Invalid SoundID: %s"), *SoundID.ToString());
		return;
	}

	FStreamableManager& Streamable = UAssetManager::Get().GetStreamableManager();
	Streamable.RequestAsyncLoad(SoundData->Sound.ToSoftObjectPath(),
		[this, SoundData, VolumeMultiplier, PitchMultiplier]()
	{
		if (SoundData && SoundData->Sound.Get())
		{
			UGameplayStatics::PlaySound2D(
				GetWorld(), SoundData->Sound.Get(),
				SoundData->DefaultVolume * VolumeMultiplier,
				SoundData->DefaultPitch * PitchMultiplier);
		}
	});
}

void USoundSubsystem::PlaySoundAtLocation(FName SoundID, const FVector& Location, float VolumeMultiplier, float PitchMultiplier)
{
	const FSoundData* SoundData = GetSoundData(SoundID);
	if (!SoundData || SoundData->Sound.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("PlaySoundAtLocation: Invalid SoundID: %s"), *SoundID.ToString());
		return;
	}

	FStreamableManager& Streamable = UAssetManager::Get().GetStreamableManager();
	Streamable.RequestAsyncLoad(SoundData->Sound.ToSoftObjectPath(),
		[this, SoundData, Location, VolumeMultiplier, PitchMultiplier]()
	{
		if (SoundData && SoundData->Sound.Get())
		{
			UGameplayStatics::PlaySoundAtLocation(
			GetWorld(), SoundData->Sound.Get(),
			Location,
			SoundData->DefaultVolume * VolumeMultiplier,
			SoundData->DefaultPitch * PitchMultiplier);
		}
	});
}