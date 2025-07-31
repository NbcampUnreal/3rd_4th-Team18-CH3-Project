// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameConfigData.generated.h"

/**
 * 게임의 설정으로 필요한 데이터가 있다면 이것에 선언하고 사용
 */
UCLASS(BlueprintType)
class UGameConfigData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> StartLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> LoadingLevel;
};
