#include "UI/Widget/MainMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Core/GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (GamePlayButton)
	{
		GamePlayButton->OnClicked.AddDynamic(this,
			&UMainMenuWidget::OnGamePlayClicked);
		GamePlayButton->OnClicked.AddDynamic(this,
			&UMainMenuWidget::OnGamePlayHovered);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this,
			&UMainMenuWidget::OnExitClicked);
		ExitButton->OnHovered.AddDynamic(this,
			&UMainMenuWidget::OnExitHovered);
	}
}

void UMainMenuWidget::OnGamePlayClicked()
{
	UGameplayStatics::OpenLevel(this, FName("FirstLevel"));
}

void UMainMenuWidget::OnExitClicked()
{
	UWorld* World = GetWorld();
	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	UKismetSystemLibrary::QuitGame(World, PC, EQuitPreference::Quit, false);
}

void UMainMenuWidget::OnGamePlayHovered()
{
	ResetButtonColors();

	if (GamePlayText)
	{
		GamePlayText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
	}

	if (DescriptionText)
	{
		DescriptionText->SetText(FText::FromString(TEXT("Play Project_Room Game")));
		DescriptionText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainMenuWidget::OnExitHovered()
{
	ResetButtonColors();

	if (ExitText)
	{
		ExitText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
	}

	if (DescriptionText)
	{
		DescriptionText->SetText(FText::FromString(TEXT("Exit Game")));
		DescriptionText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainMenuWidget::ResetButtonColors()
{
	if (GamePlayText)
	{
		GamePlayText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}

	if (ExitText)
	{
		ExitText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}
}