#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"
#include "BulletData.generated.h"

USTRUCT(BlueprintType)
struct FBulletData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
    float InitialSpeed = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
    float MaxSpeed = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
    float AutoDestroyTime = 0.0f;

    // UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
    // TSoftObjectPtr<UStaticMesh> Mesh = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
    TSoftClassPtr<AActor> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
    TSoftObjectPtr<UTexture2D> Icon = nullptr;
};
