
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
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> ToggleInventoryAction;

	// 트레이스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float InteractTraceLength = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TEnumAsByte<ECollisionChannel> InteractTraceChannel = ECC_GameTraceChannel1;

	// UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<URoomHUD> HUDClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<URoomHUD> HUDWidget;
	
private:
	// 인풋 바인딩 함수.
	void MoveByInput(const FInputActionValue& Value);
	void LookByInput(const FInputActionValue& Value);
	void InteractByInput(const FInputActionValue& Value);
	void ToggleInventoryByInput();
	void UpdateInteractableTarget();
	void UpdateFocus();
	void UpdateInteractableMessage();
	
	TWeakObjectPtr<USceneComponent> CameraRef;
	TWeakObjectPtr<AActor> CurrentTargetActor;
	TWeakObjectPtr<AActor> PreviousTargetActor;

	
};
