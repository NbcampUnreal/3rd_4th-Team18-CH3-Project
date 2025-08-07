#include "UI/UISubsystem.h"
#include "UI/Widget/MainMenuWidget.h"
#include "UI/Widget/PauseMenuWidget.h"
#include "UI/Widget/HUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UISubsystem)

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
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
    if (!MainMenuWidget.IsValid() && MainMenuWidgetClass.IsValid())
    {
        TSubclassOf<UMainMenuWidget> WidgetClass = MainMenuWidgetClass.LoadSynchronous();
        if (WidgetClass)
        {
            MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), WidgetClass);
        }
    }

    if (MainMenuWidget.IsValid())
    {
        MainMenuWidget->AddToViewport();
        SetUIInputMode();
    }
}

void UUISubsystem::ShowPauseMenu()
{
    if (!PauseMenuWidget.IsValid() && PauseMenuWidgetClass.IsValid())
    {
        TSubclassOf<UPauseMenuWidget> WidgetClass = PauseMenuWidgetClass.LoadSynchronous();
        if (WidgetClass)
        {
            PauseMenuWidget = CreateWidget<UPauseMenuWidget>(GetWorld(), WidgetClass);
        }
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
    if (!HUDWidget.IsValid() && HUDWidgetClass.IsValid())
    {
        TSubclassOf<UHUDWidget> WidgetClass = HUDWidgetClass.LoadSynchronous();
        if (WidgetClass)
        {
            HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), WidgetClass);
        }
    }

    if (HUDWidget.IsValid())
        HUDWidget->AddToViewport();
}

void UUISubsystem::HideHUD()
{
    if (HUDWidget.IsValid())
        HUDWidget->RemoveFromParent();
}

void UUISubsystem::UpdateAmmo(int32 Current, int32 Total)
{
    if (HUDWidget.IsValid())
        HUDWidget->UpdateWeaponInfo(Current, Total);
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