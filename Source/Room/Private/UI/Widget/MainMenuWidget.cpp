#include "UI/Widget/MainMenuWidget.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Core/GameManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MainMenuWidget)

void UMainMenuWidget::StartGame()
{
	GetGameInstance<UGameManager>()->StartGame();
}

void UMainMenuWidget::ExitGame()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}