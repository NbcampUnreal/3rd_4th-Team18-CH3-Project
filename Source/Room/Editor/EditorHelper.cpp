// Fill out your copyright notice in the Description page of Project Settings.


// EditorHelper.cpp
#include "EditorHelper.h"

#include "EngineUtils.h"
#include "Interface/HasRoomDataInterface.h"
#include "Interface/PoolableInterface.h"
#include "StaticData/RoomData.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

UWorld* UEditorHelper::GetEditorWorld()
{
#if WITH_EDITOR
	if (GEditor)
	{
		return GEditor->GetEditorWorldContext().World();
	}
#endif
	return nullptr;
}

bool UEditorHelper::CollectRoomDataToDataTable(UDataTable* TargetDataTable)
{
	if (!TargetDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("CollectRoomDataToDataTable: TargetDataTable is null"));
		return false;
	}

	
#if WITH_EDITOR
    UWorld* EditorWorld = nullptr;
    if (GEditor)
    {
        EditorWorld = GEditor->GetEditorWorldContext().World();
    }
    if (!EditorWorld)
    {
        UE_LOG(LogTemp, Warning, TEXT("CollectRoomDataToDataTable: Cannot get Editor World"));
        return false;
    }

    FRoomData NewRoomData;
    NewRoomData.Level = EditorWorld->GetCurrentLevel()->GetOuter()->GetPathName(); // or just EditorWorld->PersistentLevel ?

    // 중복 방지용 세트
    TSet<TSoftObjectPtr<UObject>> UniquePreloadAssets;
    TSet<FName> UniquePoolableClassNames;

    // 월드 내 모든 액터 순회
    for (TActorIterator<AActor> It(EditorWorld); It; ++It)
    {
        AActor* Actor = *It;

        // IHasRoomDataInterface 구현 여부 확인
        if (Actor->GetClass()->ImplementsInterface(UHasRoomDataInterface::StaticClass()))
        {
            // 인터페이스 호출로 프리로드 에셋 리스트 얻기
            TArray<TSoftObjectPtr<UObject>> Assets = IHasRoomDataInterface::Execute_GetPreloadAssetList(Actor);
            for (auto& Asset : Assets)
            {
                UniquePreloadAssets.Add(Asset);
            }

            // IPoolableInterface 구현 여부 확인
            if (Actor->GetClass()->ImplementsInterface(UPoolableInterface::StaticClass()))
            {
                // 중복 체크 후 추가 (클래스명 기준)
                FName ClassName = Actor->GetClass()->GetFName();
                if (!UniquePoolableClassNames.Contains(ClassName))
                {
                    UniquePoolableClassNames.Add(ClassName);

                    // PoolAbleActorInfos에 추가 (예시는 클래스명만 저장, 필요시 FPoolableActorData를 적절히 채우세요)
                    FPoolableActorData PoolData;
                    PoolData.ActorClass = Actor->GetClass();
                    NewRoomData.PoolAbleActorInfos.Add(PoolData);
                }
            }
        }
    }

    // 중복 제거된 에셋을 배열로 변환 후 할당
    NewRoomData.PreloadAssets = UniquePreloadAssets.Array();

    // DataTable에 새 행으로 추가 (RowName은 임의로 Level 이름 등으로 생성)
    FName NewRowName = FName(*FString::Printf(TEXT("Room_%s"), *EditorWorld->GetMapName()));

    // DataTable이 FRoomData 타입인지 체크
    if (TargetDataTable->GetRowStruct() == FRoomData::StaticStruct())
    {
        TargetDataTable->AddRow(NewRowName, NewRoomData);
        UE_LOG(LogTemp, Log, TEXT("Added RoomData row %s to DataTable"), *NewRowName.ToString());
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetDataTable row struct mismatch"));
        return false;
    }

#else
    UE_LOG(LogTemp, Warning, TEXT("CollectRoomDataToDataTable only available in editor"));
    return false;
#endif
}

static void ExtractSoftReferencesFromObject(UObject* Object, TSet<TSoftObjectPtr<UObject>>& OutSoftRefs)
{
    if (!Object) return;

    for (TFieldIterator<FProperty> PropIt(Object->GetClass()); PropIt; ++PropIt)
    {
        FProperty* Property = *PropIt;

        if (FSoftObjectProperty* SoftProp = CastField<FSoftObjectProperty>(Property))
        {
            // SoftObjectPtr 값 획득 (경로만 접근, 실제 로드는 안 됨)
            FSoftObjectPtr SoftPtr = SoftProp->GetPropertyValue_InContainer(Object);
            if (SoftPtr.IsValid())
            {
                OutSoftRefs.Add(TSoftObjectPtr<UObject>(SoftPtr.ToSoftObjectPath()));
            }
        }
        else if (FObjectProperty* ObjProp = CastField<FObjectProperty>(Property))
        {
            // 하드 레퍼런스, GetObjectPropertyValue_InContainer 호출 시 실제 로드 가능
            UObject* HardObj = ObjProp->GetObjectPropertyValue_InContainer(Object);
            if (HardObj)
            {
                OutSoftRefs.Add(TSoftObjectPtr<UObject>(HardObj));
            }
        }
    }
}

#if WITH_EDITOR
TArray<TSoftObjectPtr<UObject>> UEditorHelper::CollectSoftReferences(UObject* Object)
{
    TSet<TSoftObjectPtr<UObject>> UniqueSoftRefs;

    if (!Object) return UniqueSoftRefs.Array();

    ExtractSoftReferencesFromObject(Object, UniqueSoftRefs);

    if (AActor* Actor = Cast<AActor>(Object))
    {
        // 컴포넌트 검사 추가
        for (UActorComponent* Component : Actor->GetComponents())
        {
            ExtractSoftReferencesFromObject(Component, UniqueSoftRefs);
        }
    }

    return UniqueSoftRefs.Array();
}

TArray<TSoftObjectPtr<UObject>> UEditorHelper::CollectSoftReferencesFromSoftObject(const TSoftObjectPtr<UObject>& SoftObjectPtr)
{
    UObject* Obj = SoftObjectPtr.Get();
    if (!Obj) return TArray<TSoftObjectPtr<UObject>>();

    return CollectSoftReferences(Obj);
}

#else

TArray<TSoftObjectPtr<UObject>> UEditorHelper::CollectSoftReferences(UObject* Object)
{
    return TArray<TSoftObjectPtr<UObject>>();
}

TArray<TSoftObjectPtr<UObject>> UEditorHelper::CollectSoftReferencesFromSoftObject(const TSoftObjectPtr<UObject>& SoftObjectPtr)
{
    return TArray<TSoftObjectPtr<UObject>>();
}

#endif // WITH_EDITOR

