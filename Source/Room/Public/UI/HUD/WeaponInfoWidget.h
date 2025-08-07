#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

class UTextBlock;

#include "WeaponInfoWidget.generated.h"



UCLASS()
class ROOM_API UWeaponInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateAmmo(int32 CurrentAmmo, int32 TotalAmmo);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentAmmoText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalAmmoText;

	UFUNCTION(BlueprintImplementableEvent, Category = "AmmoSegment")
	void OnAmmoUpdated(int32 CurrentAmmo);
};