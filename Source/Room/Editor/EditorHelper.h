// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorHelper.generated.h"

/**
 * 
 */
UCLASS()
class ROOM_API UEditorHelper : public UBlueprintFunctionLibrary 
{
	GENERATED_BODY()

public:
	// 에디터 모드에서 현재 활성화된 월드 반환
	UFUNCTION(BlueprintCallable, CallInEditor, Category="EditorHelper")
	static UWorld* GetEditorWorld();
	
	UFUNCTION(BlueprintCallable, Category="RoomData|Editor")
	static bool CollectRoomDataToDataTable(UDataTable* TargetDataTable);

	UFUNCTION(BlueprintCallable, Category="RoomData|Editor")
	static TArray<TSoftObjectPtr<UObject>> CollectSoftReferences(UObject* Object);

	UFUNCTION(BlueprintCallable, Category="RoomData|Editor")
	static TArray<TSoftObjectPtr<UObject>> CollectSoftReferencesFromSoftObject(const TSoftObjectPtr<UObject>& SoftObjectPtr);
};
