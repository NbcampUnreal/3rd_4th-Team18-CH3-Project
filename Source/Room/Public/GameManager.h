

#pragma once

#include "CoreMinimal.h"
#include "GameManager.generated.h"

/**
 * 
 */
UCLASS()
class ROOM_API UGameManager : public UGameInstance
{
	GENERATED_BODY()
	
	
public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void ReturnToTitle();

private:
	
};
