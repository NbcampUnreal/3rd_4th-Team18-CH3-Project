#include "UI/UISubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UISubsystem)

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UUISubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UUISubsystem::ShowHUD() {}
void UUISubsystem::ShowMainMenu() {}
void UUISubsystem::ShowPauseMenu() {}
void UUISubsystem::ShowInventory() {}
void UUISubsystem::HideCurrentUI() {}

void UUISubsystem::UpdateHealth(float Current, float Max) {}
void UUISubsystem::UpdateAmmo(int32 Current, int32 Max) {}
void UUISubsystem::UpdateScore(int32 Score) {}
void UUISubsystem::UpdateMission(const FString& Name, float Progress) {}
void UUISubsystem::AddKillLog(const FString& Killer, const FString& Victim) {}

void UUISubsystem::ShowHitMarker() {}
void UUISubsystem::ShowDamageIndicator(float FromAngle) {}
void UUISubsystem::ShowKillConfirm() {}

void UUISubsystem::ShowWidget(TSoftClassPtr<UUserWidget> WidgetClass, TObjectPtr<UUserWidget>& OutInstance)
{
	// TODO: later
}