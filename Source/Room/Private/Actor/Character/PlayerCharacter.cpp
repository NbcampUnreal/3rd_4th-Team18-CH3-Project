#include "Actor/Character/PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	ActorTag = GameDefine::PlayerTag;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent()); 
	
	SpringArm->TargetArmLength = 300.0f; 
    SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0.0f, 100.0f, 0.0f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	PlayerAttackComponent = CreateDefaultSubobject<UPlayerAttackComponent>(TEXT("PlayerAttackComponent"));
	PlayerAttackComponent->SetupAttachment(GetMesh(), FName("Muzzle"));

	bUseControllerRotationYaw = true;
	

	
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(InputConfig.DefaultMappingContext, 0);
			}
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(InputConfig.MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInput->BindAction(InputConfig.JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInput->BindAction(InputConfig.JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInput->BindAction(InputConfig.AimAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInput->BindAction(InputConfig.FireAction,  ETriggerEvent::Started, this, &APlayerCharacter::StartFire);
		EnhancedInput->BindAction(InputConfig.FireAction,ETriggerEvent::Completed, this, &APlayerCharacter::StopFire);
	}
}

void APlayerCharacter::HandleDeath()
{
	Super::HandleDeath();
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->DisableInput(PC);  
	}
}


void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{

		FRotator ControlRotation = Controller->GetControlRotation();
		
		ControlRotation.Pitch = 0.f;
		ControlRotation.Roll = 0.f;
		
		const FVector Forward = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(Forward, MovementVector.Y);
		AddMovementInput(Right, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(-LookAxisVector.Y);
}
void APlayerCharacter::StartFire()
{
	if (PlayerAttackComponent)
	{
		PlayerAttackComponent->StartFire();
	}
}
void APlayerCharacter::StopFire()
{
	if (PlayerAttackComponent)
	{
		PlayerAttackComponent->StopFire();
	}
}