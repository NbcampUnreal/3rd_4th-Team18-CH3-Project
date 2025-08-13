// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemSystem/Structure/BulletItemInfo.h"
#include "ProjectileDataReciever.generated.h"

UINTERFACE()
class UProjectileDataReciever : public UInterface
{
	GENERATED_BODY()
};

class ROOM_API IProjectileDataReciever
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void SetProjectileMoveData(const FBulletItemInfo& BulletInfo);
};
