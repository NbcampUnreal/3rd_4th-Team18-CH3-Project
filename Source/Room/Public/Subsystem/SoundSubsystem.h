#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundSubsystem.generated.h"

struct FSoundData;
class UAudioComponent;

UCLASS()
class ROOM_API USoundSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayBGM(FName SoundID, float FadeInDuration = 1.0f);
	void OnBGM_Loaded(const FSoundData* SoundData, float FadeInDuration = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void StopBGM(float FadeOutDuration = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlaySound2D(FName SoundID, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlaySoundAtLocation(FName SoundID, const FVector& Location, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f);

private:
	const FSoundData* GetSoundData(FName SoundID) const;

	UPROPERTY()
	TObjectPtr<class UDataTable> SoundDataTable;

	UPROPERTY()
	TObjectPtr<UAudioComponent> BGMComponent;

	FName CurrentBGM_ID;
};