#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StaticData.generated.h"

//다양한 데이터를 데이터 테이블로 관리하기 위한 구조체들을 정의
USTRUCT(BlueprintType)
struct FStaticData : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ID")
    int32 ID = 0;

};
