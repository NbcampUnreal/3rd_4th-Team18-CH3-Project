
#include "ItemSystem/TestFramework/ItemPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/WeaponComponent.h"
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
}
void AItemPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&ThisClass::MoveByInput);
	EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered,this,&ThisClass::LookByInput);
	EnhancedInputComponent->BindAction(ToggleInventoryAction,ETriggerEvent::Started,this,&ThisClass::ToggleInventoryByInput);
	EnhancedInputComponent->BindAction(FireAction,ETriggerEvent::Started,this,&ThisClass::FireByInput);
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

void AItemPlayerController::ToggleInventoryByInput()
{
	bool bInventoryOn = HUDWidget->ToggleInventoryWidget();
	if (bInventoryOn)
	{
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}

void AItemPlayerController::FireByInput()
{
	auto WeaponComp = GetPawn()->FindComponentByClass<UWeaponComponent>();
	if (WeaponComp)
	{
		WeaponComp->Fire();
	}
}
