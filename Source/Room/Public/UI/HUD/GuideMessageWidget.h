// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GuideMessageWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class ROOM_API UGuideMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GuideTextBlock;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ShowAnim;

public:
	UFUNCTION(BlueprintCallable)
	void ShowGuideMessage(const FText& Message);
};
