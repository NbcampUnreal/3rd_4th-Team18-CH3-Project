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
    InitializeWidgetClasses();
}

void UUISubsystem::Deinitialize()
{
    MainMenuWidget.Reset();
    PauseMenuWidget.Reset();
    HUDWidget.Reset();
}

void UUISubsystem::InitializeWidgetClasses()
{
    static ConstructorHelpers::FClassFinder<UMainMenuWidget> MainMenuBP(TEXT("/Game/UI/WBP_MainMenu"));
    static ConstructorHelpers::FClassFinder<UPauseMenuWidget> PauseMenuBP(TEXT("/Game/UI/WBP_PauseMenu"));
    static ConstructorHelpers::FClassFinder<UHUDWidget> HUDWidgetBP(TEXT("/Game/UI/WBP_HUD"));

    if (MainMenuBP.Succeeded()) MainMenuWidgetClass = MainMenuBP.Class;
    if (PauseMenuBP.Succeeded()) PauseMenuWidgetClass = PauseMenuBP.Class;
    if (HUDWidgetBP.Succeeded()) HUDWidgetClass = HUDWidgetBP.Class;
}

void UUISubsystem::ShowMainMenu()
{
    if (!MainMenuWidget.IsValid() && MainMenuWidgetClass)
        MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);

    if (MainMenuWidget.IsValid())
        MainMenuWidget->AddToViewport();
}

void UUISubsystem::ShowPauseMenu()
{
    UGameplayStatics::SetGamePaused(GetWorld(), true);

    if (HUDWidget.IsValid())
        HUDWidget->RemoveFromParent();

    if (!PauseMenuWidget.IsValid() && PauseMenuWidgetClass)
        PauseMenuWidget = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseMenuWidgetClass);

    if (PauseMenuWidget.IsValid())
        PauseMenuWidget->AddToViewport();
}

void UUISubsystem::HidePauseMenu()
{
    if (PauseMenuWidget.IsValid())
        PauseMenuWidget->RemoveFromParent();

    UGameplayStatics::SetGamePaused(GetWorld(), false);

    if (HUDWidget.IsValid())
        HUDWidget->AddToViewport();
}

void UUISubsystem::ShowHUD()
{
    if (!HUDWidget.IsValid() && HUDWidgetClass)
        HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), HUDWidgetClass);

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
        HUDWidget->UpdateHealth(HealthRatio);
}