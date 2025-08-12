#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystem/RoomSubsystem.h"
#include "SoundSubsystem.generated.h"

UCLASS()
class ROOM_API USoundSubsystem : public URoomSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayBGM(const FName& BGM_ID);
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void StopBGM();
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlaySound2D(const FName& Sound_ID);
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlaySoundAtLocation(const FName& Sound_ID, const FVector& Location);
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void OnWorldCreated(UWorld* World);
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void HandleRoomStart();

private:
	UPROPERTY()
	TObjectPtr<UAudioComponent> BGM_Component;
};
