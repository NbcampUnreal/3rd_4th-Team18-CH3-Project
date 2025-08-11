#include "UI/UISubsystem.h"
#include "UI/Widget/MainMenuWidget.h"
#include "UI/Widget/PauseMenuWidget.h"
#include "UI/Widget/HUDWidget.h"
#include "UI/Widget/LoadingScreenWidget.h"
#include "UI/Widget/DamageTextActor.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

#include "Subsystem/StaticDataSubsystem.h"
#include "StaticData/StaticDataStruct.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UISubsystem)

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    const FString MainMenuPath = TEXT("/Game/UI/WBP_MainMenu.WBP_MainMenu_C");
    const FString PauseMenuPath = TEXT("/Game/UI/WBP_PauseMenu.WBP_PauseMenu_C");
    const FString HUDPath = TEXT("/Game/UI/WBP_HUD.WBP_HUD_C");
    const FString LoadingScreentPath = TEXT("/Game/UI/WBP_LoadingScreen.WBP_LoadingScreen_C");
    const FString DamageTextActorPath = TEXT("/Game/UI/Widgets/BP_DamageTextActor.BP_DamageTextActor_C");

    TSubclassOf<UMainMenuWidget> LoadedMainMenuClass = TSoftClassPtr<UMainMenuWidget>(FSoftObjectPath(MainMenuPath)).LoadSynchronous();
    TSubclassOf<UPauseMenuWidget> LoadedPauseMenuClass = TSoftClassPtr<UPauseMenuWidget>(FSoftObjectPath(PauseMenuPath)).LoadSynchronous();
    TSubclassOf<UHUDWidget> LoadedHUDClass = TSoftClassPtr<UHUDWidget>(FSoftObjectPath(HUDPath)).LoadSynchronous();
    TSubclassOf<UHUDWidget> LoadedLoadingClass = TSoftClassPtr<ULoadingScreenWidget>(FSoftObjectPath(HUDPath)).LoadSynchronous();
    TSubclassOf<ADamageTextActor> LoadedDamageTextActorClass = TSoftClassPtr<ADamageTextActor>(FSoftObjectPath(DamageTextActorPath)).LoadSynchronous();


    if (LoadedMainMenuClass)
    {
        MainMenuWidgetClass = LoadedMainMenuClass;
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("SUCCESS: MainMenu Class Loaded via C++ Hardcode."));
    }
    else
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("FATAL ERROR: MainMenu Class FAILED to load from C++ Hardcode. Check Path!"));
    }

    if (LoadedPauseMenuClass)
    {
        PauseMenuWidgetClass = LoadedPauseMenuClass;
    }

    if (LoadedHUDClass)
    {
        HUDWidgetClass = LoadedHUDClass;
    }
    if (LoadedLoadingClass)
    {
        LoadingWidgetClass = LoadedLoadingClass;
    }
    if (LoadedDamageTextActorClass)
    {
        DamageTextActorClass = LoadedDamageTextActorClass;
    }
}

void UUISubsystem::Deinitialize()
{
    MainMenuWidget.Reset();
    PauseMenuWidget.Reset();
    HUDWidget.Reset();
}

void UUISubsystem::SetUIInputMode()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        FInputModeUIOnly InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetWidgetToFocus(nullptr);
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
    }
}

void UUISubsystem::SetGameInputMode()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }
}

void UUISubsystem::ShowMainMenu()
{
    if (!MainMenuWidget.IsValid() && MainMenuWidgetClass)
    {
        MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
    }

    if (MainMenuWidget.IsValid())
    {
        MainMenuWidget->AddToViewport();
        SetUIInputMode();
    }
}

void UUISubsystem::ShowPauseMenu()
{
    if (!PauseMenuWidget.IsValid() && PauseMenuWidgetClass)
    {
        PauseMenuWidget = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseMenuWidgetClass);
    }

    if (PauseMenuWidget.IsValid())
    {
        PauseMenuWidget->AddToViewport();
        SetUIInputMode();
    }

    HideHUD();
    UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UUISubsystem::HidePauseMenu()
{
    if (PauseMenuWidget.IsValid())
    {
        PauseMenuWidget->RemoveFromParent();
    }

    ShowHUD();
    UGameplayStatics::SetGamePaused(GetWorld(), false);
    SetGameInputMode();
}

void UUISubsystem::ShowHUD()
{
    if (!HUDWidget.IsValid() && HUDWidgetClass)
    {
        HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), HUDWidgetClass);
    }

    if (HUDWidget.IsValid())
        HUDWidget->AddToViewport();
}

void UUISubsystem::HideHUD()
{
    if (HUDWidget.IsValid())
        HUDWidget->RemoveFromParent();
}

void UUISubsystem::UpdateAmmo(int32 Current,int32 Max, int32 Total)
{
    if (HUDWidget.IsValid())
        HUDWidget->UpdateWeaponInfo(Current, Max, Total);
}

void UUISubsystem::UpdateObjective(int32 RangedKill, int32 RangedTotal, int32 MeleeKill, int32 MeleeTotal)
{
    if (HUDWidget.IsValid())
        HUDWidget->UpdateObjective(RangedKill, RangedTotal, MeleeKill, MeleeTotal);
}

void UUISubsystem::UpdateHealth(float HealthRatio)
{
    if (HUDWidget.IsValid())
    {
        HUDWidget->UpdateHealth(HealthRatio);
    }
}

void UUISubsystem::ShowLoadingScreen()
{
    if (!LoadingWidget.IsValid() && LoadingWidgetClass)
    {
        LoadingWidget = CreateWidget<ULoadingScreenWidget>(GetWorld(), LoadingWidgetClass);
    }

    if (LoadingWidget.IsValid())
    {
        LoadingWidget->AddToViewport();
        SetUIInputMode();
    }
}

void UUISubsystem::HideLoadingScreen()
{
    if (LoadingWidget.IsValid())
    {
        LoadingWidget->RemoveFromParent();
    }
}

void UUISubsystem::ShowHitMarkerOnHUD()
{
    if (HUDWidget.IsValid())
    {
        HUDWidget->ShowCrosshairHitMarker();
    }
}

void UUISubsystem::ShowKillMarkerOnHUD()
{
    if (HUDWidget.IsValid())
    {
        HUDWidget->ShowKillMarker();
    }
}

void UUISubsystem::ShowDamageNumber(int32 Damage, FVector WorldLocation)
{
    if (GetWorld() && DamageTextActorClass)
    {
        ADamageTextActor* SpawnedActor = GetWorld()->SpawnActor<ADamageTextActor>(DamageTextActorClass, WorldLocation, FRotator::ZeroRotator);
        if (SpawnedActor)
        {
            SpawnedActor->SetDamage(Damage);
        }
    }
}