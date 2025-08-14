// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/LoadingLevelGameMode.h"
#include "SubSystem/LoadingSubsystem.h"
#include "Engine/GameInstance.h"
#include "UI/UISubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LoadingLevelGameMode)

void ALoadingLevelGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetGameInstance()->GetSubsystem<UUISubsystem>()->ShowLoadingScreen();

	if (UGameInstance* GI = GetGameInstance())
	{
		if (ULoadingSubsystem* Loader = GI->GetSubsystem<ULoadingSubsystem>())
		{
			Loader->StartLoadingAssets(
				FStreamableDelegate::CreateUObject(Loader, &ULoadingSubsystem::OpenTargetLevel));
		}
	}
}