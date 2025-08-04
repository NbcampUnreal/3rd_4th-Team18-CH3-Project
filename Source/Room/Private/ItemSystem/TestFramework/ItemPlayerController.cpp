
#include "ItemSystem/TestFramework/ItemPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "ItemSystem/Interfaces/Interactable.h"
#include "ItemSystem/UI/HUD/RoomHUD.h"
#include "Kismet/KismetMathLibrary.h"

AItemPlayerController::AItemPlayerController()
{
}

void AItemPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// 디폴트 인풋 맵핑.
	if (auto Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		Subsystem->AddMappingContext(DefaultInputMappingContext,0);
	}

	// 트레이스 위치를 위한 카메라 레퍼런스
	CameraRef = GetPawn()->FindComponentByClass<UCameraComponent>();

	// 허드 위젯 생성.
	if (HUDClass)
	{
		HUDWidget = CreateWidget<URoomHUD>(GetWorld(),HUDClass);
		HUDWidget->AddToViewport();
	}

	
}


void AItemPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateInteractableTarget();
	UpdateFocus();

	UpdateInteractableMessage();
}
void AItemPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&ThisClass::MoveByInput);
	EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered,this,&ThisClass::LookByInput);
	EnhancedInputComponent->BindAction(InteractAction,ETriggerEvent::Started,this,&ThisClass::InteractByInput);
	EnhancedInputComponent->BindAction(ToggleInventoryAction,ETriggerEvent::Started,this,&ThisClass::ToggleInventoryByInput);
}
void AItemPlayerController::MoveByInput(const FInputActionValue& Value)
{
	// 인풋벡터를 복사하고 크기가 1보다 크면 노말라이즈.
	FVector2D InputVector = Value.Get<FVector2D>();
	if (InputVector.Size()>1.0f)
	{
		InputVector.Normalize();
	}

	// 컨트롤 로테이션의 방향벡터 가져오기.
	FRotator RotationFoDir = GetControlRotation();
	RotationFoDir.Pitch = 0.0f;
	RotationFoDir.Roll = 0.0f;
	FVector ControlForwardVector = UKismetMathLibrary::GetForwardVector(RotationFoDir);
	FVector ControlRightVector = UKismetMathLibrary::GetRightVector(RotationFoDir);

	// 포세스된 폰에 이동 입력.
	FVector MoveVector = ControlForwardVector * InputVector.Y + ControlRightVector * InputVector.X;
	GetPawn()->AddMovementInput(MoveVector,1.0f);
}
void AItemPlayerController::LookByInput(const FInputActionValue& Value)
{
	// 카메라 회전.
	FVector2D InputVector = Value.Get<FVector2D>();
	AddYawInput(InputVector.X);
	AddPitchInput(InputVector.Y);
}
void AItemPlayerController::InteractByInput(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Interact Input"));
	if (CurrentTargetActor.IsValid())
	{
		IInteractable::Execute_Interact(CurrentTargetActor.Get());
	}
}
void AItemPlayerController::ToggleInventoryByInput()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("ToggleInventory Input"));
	HUDWidget->ToggleInventoryWidget();
}
void AItemPlayerController::UpdateInteractableTarget()
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
void AItemPlayerController::UpdateFocus()
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

void AItemPlayerController::UpdateInteractableMessage()
{
	if (CurrentTargetActor.Get())
	{
		HUDWidget->ShowInteractMessage(IInteractable::Execute_GetInteractableMessage(CurrentTargetActor.Get()));
	}
	else
	{
		HUDWidget->HideInteractMessage();
	}
}


