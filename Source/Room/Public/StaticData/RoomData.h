#pragma once

#include "CoreMinimal.h"
#include "StaticData.h"

#include "RoomData.generated.h"

USTRUCT(BlueprintType)
struct FPoolableActorData 
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pool")
    TSoftClassPtr<AActor> ActorClass = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pool")
    int32 InitialPoolSize = 0;
};


USTRUCT(BlueprintType)
struct FRoomData : public FStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UWorld> Level;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Preload")
    TArray<FPoolableActorData> PoolAbleActorInfos;
    // 기타 로딩용 데이터들 파티클, 사운드, 동적 젠 되는 몬스터 등.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preload")
	TArray<TSoftObjectPtr<UObject>> PreloadAssets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Preload|Monsters")
	TArray<FDataTableRowHandle>	Monsters; 
	// UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Preload|Item")
	// TArray<FDataTableRowHandle>	Items;


    // AI가 플레이어를 감지할 수 있는 최대 거리 (단위: cm)
    // 예: 700.f이면 7m 거리에서 플레이어를 인식
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float AISightRadius = 500.f;

    // 플레이어가 AI의 시야에서 벗어난 후에도 추적을 유지할 수 있는 거리 (단위: cm)
    // AISightRadius보다 커야 하며, 보통 200~500cm 차이를 둔다고 함.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float AILoseSightRadius = 700.f;
};
