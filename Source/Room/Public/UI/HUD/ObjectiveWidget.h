#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveWidget.generated.h"

class UTextBlock;

UCLASS()
class ROOM_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget)) UTextBlock* RangedText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* MeleeText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* StatusText;

public:
	void UpdateObjective(
		int32 RangedKills,
		int32 RangedTotal,
		int32 MeleeKills,
		int32 MeleeTotal);
};
