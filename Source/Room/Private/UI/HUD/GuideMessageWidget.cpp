// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/GuideMessageWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

void UGuideMessageWidget::ShowGuideMessage(const FText& Message)
{
	if (GuideTextBlock && ShowAnim)
	{
		GuideTextBlock->SetText(Message);
		PlayAnimation(ShowAnim);
	}
}
