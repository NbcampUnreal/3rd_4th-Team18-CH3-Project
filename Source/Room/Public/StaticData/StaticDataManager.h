#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

struct FDataManagerBase
{
	virtual ~FDataManagerBase() = default;
	virtual void Load(const UDataTable* DataTable) = 0;
};

template<typename TStruct>
struct TStaticDataManager : public FDataManagerBase
{
private:
	TMap<int32, TStruct> DataMap;
	TFunction<int32(const TStruct&)> GetKeyFunc;

public:
	TStaticDataManager(TFunction<int32(const TStruct&)> InGetKeyFunc)
		: GetKeyFunc(InGetKeyFunc)
	{
	}

	virtual void Load(const UDataTable* DataTable) override
	{
		if (!DataTable || !GetKeyFunc) return;

		TArray<FName> RowNames = DataTable->GetRowNames();
		DataMap.Empty();

		for (const FName& RowName : RowNames)
		{
			TStruct* Row = DataTable->FindRow<TStruct>(RowName, TEXT("StaticDataLoad"));
			if (Row)
			{
				DataMap.Add(GetKeyFunc(*Row), *Row);
			}
		}
	}

	const TStruct* GetData(int32 Key) const
	{
		return DataMap.Find(Key);
	}
};
