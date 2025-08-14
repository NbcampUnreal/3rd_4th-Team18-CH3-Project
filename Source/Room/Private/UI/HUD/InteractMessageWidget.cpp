// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/InteractMessageWidget.h"
#include "Components/TextBlock.h"

void UInteractMessageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UInteractMessageWidget::ShowMessage(const FText& Message)
{
	if (InteractTextBlock)
	{
		InteractTextBlock->SetText(Message);
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UInteractMessageWidget::HideMessage()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

