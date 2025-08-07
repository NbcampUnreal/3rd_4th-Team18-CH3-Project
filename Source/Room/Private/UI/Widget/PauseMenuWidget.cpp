#include "UI/Widget/PauseMenuWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GameManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PauseMenuWidget)

void UPauseMenuWidget::ResumeGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void UPauseMenuWidget::ReturnToMainMenu()
{
	GetGameInstance<UGameManager>()->ReturnToTitle();
}

void UPauseMenuWidget::ExitGameFromPause()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}