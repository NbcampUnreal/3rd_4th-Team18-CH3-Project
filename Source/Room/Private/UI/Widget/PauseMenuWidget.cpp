#include "UI/Widget/PauseMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GameManager.h"

void UPauseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ResumeButton)
    {
        ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
        ResumeButton->OnHovered.AddDynamic(this, &UPauseMenuWidget::OnResumeHovered);
        ResumeButton->OnUnhovered.AddDynamic(this, &UPauseMenuWidget::OnButtonUnhovered);
    }

    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnMainMenuClicked);
        MainMenuButton->OnHovered.AddDynamic(this, &UPauseMenuWidget::OnMainMenuHovered);
        MainMenuButton->OnUnhovered.AddDynamic(this, &UPauseMenuWidget::OnButtonUnhovered);
    }

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnExitClicked);
        ExitButton->OnHovered.AddDynamic(this, &UPauseMenuWidget::OnExitHovered);
        ExitButton->OnUnhovered.AddDynamic(this, &UPauseMenuWidget::OnButtonUnhovered);
    }

    if (DescriptionText)
    {
        DescriptionText->SetText(FText::GetEmpty());
    }
}

void UPauseMenuWidget::OnResumeClicked()
{
    UGameInstance* GI = GetGameInstance();
    if (GI)
    {
        UGameplayStatics::SetGamePaused(GetWorld(), false);
        RemoveFromParent();
    }
}

void UPauseMenuWidget::OnMainMenuClicked()
{
    GetGameInstance<UGameManager>()->ReturnToTitle();
	RemoveFromParent();
}

void UPauseMenuWidget::OnExitClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UPauseMenuWidget::OnResumeHovered()
{
    if (DescriptionText)
    {
        DescriptionText->SetText(FText::FromString(TEXT("Resume the game")));
    }
}

void UPauseMenuWidget::OnMainMenuHovered()
{
    if (DescriptionText)
    {
        DescriptionText->SetText(FText::FromString(TEXT("Return to the main menu")));
    }
}

void UPauseMenuWidget::OnExitHovered()
{
    if (DescriptionText)
    {
        DescriptionText->SetText(FText::FromString(TEXT("Exit the game")));
    }
}

void UPauseMenuWidget::OnButtonUnhovered()
{
    if (DescriptionText)
    {
        DescriptionText->SetText(FText::GetEmpty());
    }
}