// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractMessageWidget.generated.h"

class UTextBlock;

UCLASS()
class ROOM_API UInteractMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractTextBlock;

	virtual void NativeConstruct() override;

public:
	void ShowMessage(const FText& Message);
	void HideMessage();
};
