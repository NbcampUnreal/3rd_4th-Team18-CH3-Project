#include "ItemSystem/InteractionComponent/InteractionComponent.h"

#include "Camera/CameraComponent.h"
#include "ItemSystem/Interfaces/Interactable.h"
#include "ItemSystem/TestFramework/ItemPlayerController.h"
#include "ItemSystem/UI/HUD/RoomHUD.h"
#include "Kismet/GameplayStatics.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	CameraRef = GetOwner()->FindComponentByClass<UCameraComponent>();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateInteractableTarget();
	UpdateFocus();
	UpdateInteractableMessage();
}

void UInteractionComponent::Interact()
{
		if (CurrentTargetActor.IsValid())
		{
			IInteractable::Execute_Interact(CurrentTargetActor.Get(),GetOwner());
		}
}

void UInteractionComponent::UpdateInteractableTarget()
{
	FHitResult HitResult;
	FVector Start = CameraRef->GetComponentLocation();
	FVector End = Start + CameraRef->GetForwardVector() * InteractTraceLength;
	GetWorld()->LineTraceSingleByChannel(OUT HitResult,Start,End,InteractTraceChannel);
		
	PreviousTargetActor = CurrentTargetActor;
	if (HitResult.bBlockingHit && HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		CurrentTargetActor = HitResult.GetActor();
	}
	else
	{
		CurrentTargetActor = nullptr;	
	}
}

void UInteractionComponent::UpdateFocus()
{
	if (PreviousTargetActor != CurrentTargetActor)
	{
		if (PreviousTargetActor != nullptr)
		{
			if (PreviousTargetActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
			{
				IInteractable::Execute_OutFocus(PreviousTargetActor.Get());
			}
		}

		if (CurrentTargetActor != nullptr)
		{
			if (CurrentTargetActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
			{
				IInteractable::Execute_InFocus(CurrentTargetActor.Get());
			}
		}
	}
}

void UInteractionComponent::UpdateInteractableMessage()
{
	// TODO: 이후 UI 구조 봐서 수정 할 것.
	
	if (CurrentTargetActor.Get())
	{
		if (auto PC = Cast<AItemPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0)))
		{
			if (URoomHUD* HUDWidget = PC->GetHUDWidget())
			{
				HUDWidget->ShowInteractMessage(IInteractable::Execute_GetInteractableMessage(CurrentTargetActor.Get()));
			}
			
		}
	}
	else
	{
		if (auto PC = Cast<AItemPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0)))
		{
			if (URoomHUD* HUDWidget = PC->GetHUDWidget())
			{
				HUDWidget->HideInteractMessage();
			}
		}
	}
}

