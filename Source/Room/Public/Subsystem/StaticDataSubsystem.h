#pragma once

#include "CoreMinimal.h"
#include "Subsystem/RoomSubsystem.h"
#include "Engine/DataTable.h"
#include "StaticData/StaticDataManager.h"
#include "StaticDataSubsystem.generated.h"
/*데이터 테이블을 자동으로 로드하고 각 데이터 타입에 맞는 전용 데이터 매니저를
생성 및 등록하여 정적 데이터를 쉽게 조회할 수 있도록 관리하는 시스템/
로딩매니저에서 로드하여 사용*/
UCLASS()
class ROOM_API UStaticDataSubsystem : public URoomSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    
    //통합 데이터 테이블
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable")
    TMap<FName, TObjectPtr<UDataTable>> DataTables;
    //에디터에서 등록할 데이터 테이블 리스트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable")
    TArray<TObjectPtr<UDataTable>> DataTablesToRegister;
    //데이터 타입별 관리
    TMap<FName, TSharedPtr<FDataManagerBase>> StaticDataManagers;

    

    template<typename TStruct>
    const TStruct* GetData(int32 Key) const
    {
        //구조체 이름을 키로 사용
        FName StructName = TStruct::StaticStruct()->GetFName();

        //매니저를 찾고 다운캐스팅하여 키로 데이터 조회
        if (const TSharedPtr<FDataManagerBase>* FoundManager = StaticDataManagers.Find(StructName))
        {
            TSharedPtr<TStaticDataManager<TStruct>> Manager = StaticCastSharedPtr<TStaticDataManager<TStruct>>(*FoundManager);
            if (Manager.IsValid())
            {
                return Manager->GetData(Key);
            }
        }
        return nullptr;
    }

    template<typename TStruct>
    void RegisterDataManager(
     const FName& DataTableName,
     TFunction<int32(const TStruct&)> GetKeyFunc = [](const TStruct& Data) { return Data.ID; })
    {
        //등록된 데이터 테이블을 찾고 없으면 종료
        TObjectPtr<UDataTable>* FoundTable = DataTables.Find(DataTableName);
        if (!FoundTable || !*FoundTable) return;
    
        
        //구조체 이름을 키로 사용, 중복 방지
        FName StructName = TStruct::StaticStruct()->GetFName();
        if (StaticDataManagers.Contains(StructName)) return;
        
        //데이터 매니저 생성, 맵에 등록
        TSharedPtr<FDataManagerBase> NewManager = MakeShared<TStaticDataManager<TStruct>>(GetKeyFunc);
        NewManager->Load(*FoundTable);
        StaticDataManagers.Add(StructName, NewManager);
    }

    //데이터 테이블 이름 : DT_???DataTable로 통일해야 함
    template<typename TStruct>
    FName GetDTName()
    {
        FString S = TStruct::StaticStruct()->GetFName().ToString();
        if (S.StartsWith("F")) S.RemoveAt(0);
        return FName(*("DT_" + S + "Table"));
    }
};
