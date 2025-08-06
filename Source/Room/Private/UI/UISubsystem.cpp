#include "UI/UISubsystem.h"
#include "UI/Widget/MainMenuWidget.h"

#include "Blueprint/UserWidget.h"

#include "Kismet/GameplayStatics.h"

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    InitializeWidgetClasses();
}

void UUISubsystem::Deinitialize()
{
    MainMenuWidget.Reset();
    //PauseMenuWidget.Reset();
    /*HUDWidget.Reset();*/
}

void UUISubsystem::InitializeWidgetClasses()
{
    static ConstructorHelpers::FClassFinder<UMainMenuWidget> MainMenuBP(TEXT("/Game/UI/WBP_MainMenu"));
    //static ConstructorHelpers::FClassFinder<UPauseMenuWidget> PauseMenuBP(TEXT("/Game/UI/WBP_PauseMenu"));
    /*static ConstructorHelpers::FClassFinder<UHUDWidget> HUDWidgetBP(TEXT("/Game/UI/WBP_HUD"));*/

    if (MainMenuBP.Succeeded()) MainMenuWidgetClass = MainMenuBP.Class;
    //if (PauseMenuBP.Succeeded()) PauseMenuWidgetClass = PauseMenuBP.Class;
    /*if (HUDWidgetBP.Succeeded()) HUDWidgetClass = HUDWidgetBP.Class;*/
}

void UUISubsystem::ShowMainMenu()
{
    if (!MainMenuWidget.IsValid() && MainMenuWidgetClass)
        MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);

    if (MainMenuWidget.IsValid())
        MainMenuWidget->AddToViewport();
}

