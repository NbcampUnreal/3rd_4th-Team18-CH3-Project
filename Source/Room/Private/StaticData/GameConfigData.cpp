// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticData/GameConfigData.h"

#include "Engine/AssetManager.h"

const FPrimaryAssetId UGameConfigData::PrimaryId = FPrimaryAssetId("GameConfigData", "DA_GameConfigData");


UGameConfigData* UGameConfigData::Get()
{
	static TWeakObjectPtr<UGameConfigData> CachedConfig;

	if (CachedConfig.IsValid())
	{
		return CachedConfig.Get();
	}
	
	UAssetManager& Manager = UAssetManager::Get();

	UObject* AssetObj = Manager.GetPrimaryAssetObject(PrimaryId);
	if (!AssetObj)
	{
		AssetObj = Manager.GetStreamableManager().LoadSynchronous(Manager.GetPrimaryAssetPath(PrimaryId));
	}

	CachedConfig = Cast<UGameConfigData>(AssetObj);
	return CachedConfig.Get();
}