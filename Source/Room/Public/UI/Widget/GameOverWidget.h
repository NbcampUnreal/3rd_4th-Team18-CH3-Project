// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROOM_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void Retry();

	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void ReturnToMainMenu();

	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void ExitGame();

	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void SetupGameResult(bool bIsClear, int32 FinalScore);

	UFUNCTION(BlueprintImplementableEvent, Category ="GameOver")
	void OnSetupGameResult(bool bIsClear, int32 FinalScore);
};
