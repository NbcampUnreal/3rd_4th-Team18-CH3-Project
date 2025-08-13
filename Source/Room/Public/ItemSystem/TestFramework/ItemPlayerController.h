
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ItemPlayerController.generated.h"

class URoomHUD;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UUserWidget;

UCLASS()
class ROOM_API AItemPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AItemPlayerController();
	URoomHUD* GetHUDWidget() const {return HUDWidget;}
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;
	
	// 인풋 에셋.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> ToggleInventoryAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> FireAction;
	
	// UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<URoomHUD> HUDClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<URoomHUD> HUDWidget;
	
private:
	// 인풋 바인딩 함수.
	void MoveByInput(const FInputActionValue& Value);
	void LookByInput(const FInputActionValue& Value);
	void ToggleInventoryByInput();
	void FireByInput();

	
};
