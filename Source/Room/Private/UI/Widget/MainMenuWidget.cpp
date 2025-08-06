#include "UI/Widget/MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GameManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MainMenuWidget)

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (GameStartButton)
    {
        GameStartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnGameStartClicked);
        GameStartButton->OnHovered.AddDynamic(this, &UMainMenuWidget::OnGameStartHovered);
        GameStartButton->OnUnhovered.AddDynamic(this, &UMainMenuWidget::OnButtonUnhovered);
    }

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitClicked);
        ExitButton->OnHovered.AddDynamic(this, &UMainMenuWidget::OnExitHovered);
        ExitButton->OnUnhovered.AddDynamic(this, &UMainMenuWidget::OnButtonUnhovered);
    }

    if (DescriptionText)
    {
        DescriptionText->SetText(FText::GetEmpty());
    }
}

void UMainMenuWidget::OnGameStartClicked()
{
	GetGameInstance<UGameManager>()->StartGame();
}

void UMainMenuWidget::OnExitClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UMainMenuWidget::OnGameStartHovered()
{
    if (DescriptionText)
    {
        DescriptionText->SetText(FText::FromString(TEXT("Play Project_Room Game")));
    }
}

void UMainMenuWidget::OnExitHovered()
{
    if (DescriptionText)
    {
        DescriptionText->SetText(FText::FromString(TEXT("Exit the game")));
    }
}

void UMainMenuWidget::OnButtonUnhovered()
{
    if (DescriptionText)
    {
        DescriptionText->SetText(FText::GetEmpty());
    }
}