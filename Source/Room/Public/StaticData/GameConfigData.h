// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameConfigData.generated.h"

/**
 * 게임의 설정으로 필요한 데이터가 있다면 여기에 선언하고 사용
 * 전역적으로 딱 하나만 존재하는 PrimaryAsset
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> TitleLevel;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	// 전역 접근 함수
	UFUNCTION(BlueprintCallable, Category="Game Config")
	static UGameConfigData* Get();
	
};
