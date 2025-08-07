#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StaticDataStruct.h" // FPoolableActorData를 위해 포함

//데이터 테이블에서 구조체 데이터를 로드하고 키를 사용하여 데이터를 조회하는 매니저
struct FDataManagerBase
{
	virtual ~FDataManagerBase() = default;
	virtual void Load(const UDataTable* DataTable) = 0;
	virtual const FTableRowBase* GetData(int32 Key) const = 0;
	virtual const TMap<int32, const FTableRowBase*>& GetAllData() const = 0;
};

template<typename TStruct>
struct TStaticDataManager : public FDataManagerBase
{
private:
	// 주 데이터 저장소. 데이터의 실제 소유권을 가집니다.
	TMap<int32, TStruct> DataStore;
	// 기본 키(int32)를 추출하는 람다 함수
	TFunction<int32(const TStruct&)> GetPrimaryKeyFunc;

	// 보조 인덱스 맵들을 저장하는 컨테이너. 타입 소거(Type Erasure)를 위해 TSharedPtr<void>를 사용합니다.
	TMap<FName, TSharedPtr<void>> SecondaryIndexMaps;

	// GetAllData() 호출 시 반환할 맵. 캐시하여 사용합니다.
	mutable TMap<int32, const FTableRowBase*> AllDataMap;
	// 캐시가 유효한지 나타내는 플래그
	mutable bool bIsAllDataMapDirty = true;

public:
	TStaticDataManager(TFunction<int32(const TStruct&)> InGetKeyFunc)
		: GetPrimaryKeyFunc(InGetKeyFunc)
	{
	}

	// 데이터 테이블에서 데이터를 로드하여 DataStore를 채웁니다.
	virtual void Load(const UDataTable* DataTable) override
	{
		DataStore.Empty();
		SecondaryIndexMaps.Empty(); // 인덱스도 함께 초기화
		if (DataTable)
		{
			TArray<FName> RowNames = DataTable->GetRowNames();
			for (const FName& RowName : RowNames)
			{
				TStruct* Row = DataTable->FindRow<TStruct>(RowName, TEXT(""));
				if (Row)
				{
					DataStore.Add(GetPrimaryKeyFunc(*Row), *Row);
				}
			}
		}
		bIsAllDataMapDirty = true; // 데이터가 변경되었으므로 캐시를 무효화합니다.
	}

	// 기본 키(int32)로 데이터를 조회합니다.
	virtual const FTableRowBase* GetData(int32 Key) const override
	{
		// DataStore에서 직접 찾아 반환합니다.
		return DataStore.Find(Key);
	}
    
	const TStruct* GetTypedData(int32 Key) const
	{
		return DataStore.Find(Key);
	}

	// 모든 데이터를 조회합니다. 캐시된 맵을 사용하여 성능을 최적화합니다.
	virtual const TMap<int32, const FTableRowBase*>& GetAllData() const override
	{
		if (bIsAllDataMapDirty)
		{
			AllDataMap.Empty();
			for (const auto& Pair : DataStore)
			{
				AllDataMap.Add(Pair.Key, &Pair.Value);
			}
			bIsAllDataMapDirty = false; // 캐시를 최신 상태로 업데이트합니다.
		}
		return AllDataMap;
	}

	// 새로운 보조 인덱스를 추가하는 템플릿 함수
	template<typename TKey>
	void AddIndex(const FName& IndexName, TFunction<TKey(const TStruct&)> GetKeyFunc)
	{
		// 지정된 타입의 키와 데이터 포인터를 매핑하는 새로운 맵을 생성합니다.
		auto NewIndexMap = MakeShared<TMap<TKey, const TStruct*>>();
		// DataStore를 순회하며 인덱스 맵을 채웁니다.
		for (const auto& Pair : DataStore)
		{
			NewIndexMap->Add(GetKeyFunc(Pair.Value), &Pair.Value);
		}
		// 생성된 인덱스 맵을 SecondaryIndexMaps에 추가합니다.
		SecondaryIndexMaps.Add(IndexName, NewIndexMap);
	}

	// 보조 키를 사용하여 데이터를 조회하는 템플릿 함수
	template<typename TKey>
	const TStruct* GetDataByKey(const FName& IndexName, const TKey& Key) const
	{
		// 인덱스 이름으로 해당 인덱스 맵을 찾습니다.
		if (const TSharedPtr<void>* FoundMapPtr = SecondaryIndexMaps.Find(IndexName))
		{
			// TSharedPtr<void>를 원래 타입(TMap<TKey, const TStruct*>)으로 캐스팅합니다.
			const auto IndexMap = StaticCastSharedPtr<TMap<TKey, const TStruct*>>(*FoundMapPtr);
			// 맵에서 키를 사용하여 데이터 포인터를 찾습니다.
			if (const TStruct* const* FoundData = IndexMap->Find(Key))
			{
				return *FoundData;
			}
		}
		return nullptr; // 데이터를 찾지 못하면 nullptr을 반환합니다.
	}
};