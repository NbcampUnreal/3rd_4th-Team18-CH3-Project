// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/GameOverWidget.h"
#include "Core/GameManager.h"
#include "Kismet/KismetSystemLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameOverWidget)

void UGameOverWidget::Retry()
{
	if (UGameManager* GM = GetGameInstance<UGameManager>())
	{
		GM->StartGame();
	}
}

void UGameOverWidget::ReturnToMainMenu()
{
	if (UGameManager* GM = GetGameInstance<UGameManager>())
	{
		GM->ReturnToTitle();
	}
}

void UGameOverWidget::ExitGame()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UGameOverWidget::SetupGameResult(bool bIsClear, int32 FinalScore)
{
	OnSetupGameResult(bIsClear, FinalScore);
}