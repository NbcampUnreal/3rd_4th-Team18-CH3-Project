#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable)
class ROOM_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractTraceLength = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TEnumAsByte<ECollisionChannel> InteractTraceChannel = ECC_GameTraceChannel1;

private:
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();
	void UpdateInteractableTarget();
	void UpdateFocus();
	void UpdateInteractableMessage();

	TWeakObjectPtr<USceneComponent> CameraRef;
	TWeakObjectPtr<AActor> CurrentTargetActor;
	TWeakObjectPtr<AActor> PreviousTargetActor;
	
};
