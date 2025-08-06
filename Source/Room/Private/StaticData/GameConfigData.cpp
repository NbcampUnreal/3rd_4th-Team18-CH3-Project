// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticData/GameConfigData.h"

#include "Engine/AssetManager.h"


FPrimaryAssetId UGameConfigData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("GameConfigData"), GetFName());
}

UGameConfigData* UGameConfigData::Get()
{
	static TWeakObjectPtr<UGameConfigData> CachedConfig;

	if (CachedConfig.IsValid())
	{
		return CachedConfig.Get();
	}
	
	const FPrimaryAssetId PrimaryId = FPrimaryAssetId(TEXT("GameConfigData"), TEXT("DA_GameConfig"));
	UAssetManager& Manager = UAssetManager::Get();
	UObject* AssetObj = Manager.GetPrimaryAssetObject(PrimaryId);
	if (!AssetObj)
	{
		auto Path = Manager.GetPrimaryAssetPath(PrimaryId);
		AssetObj = Manager.GetStreamableManager().LoadSynchronous(Path);
	}

	CachedConfig = Cast<UGameConfigData>(AssetObj);
	return CachedConfig.Get();
}
