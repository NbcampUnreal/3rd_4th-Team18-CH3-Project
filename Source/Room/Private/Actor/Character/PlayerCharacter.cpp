#include "Actor/Character/PlayerCharacter.h"
#include "UI/UISubsystem.h"

APlayerCharacter::APlayerCharacter()
{

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent()); 
	
	SpringArm->TargetArmLength = 400.0f; 
    SpringArm->SocketOffset = FVector(0.0f, 150.0f, 0.0f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	
	PlayerAttackComponent = CreateDefaultSubobject<UPlayerAttackComponent>(TEXT("PlayerAttackComponent"));
	PlayerAttackComponent->SetupAttachment(GetMesh(), FName("Muzzle"));

	bUseControllerRotationYaw = true;
	

	
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	OwnedGameplayTags.AddTag(GameDefine::PlayerTag);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UISubsystem = GameInstance->GetSubsystem<UUISubsystem>();
	}

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(InputConfig.DefaultMappingContext, 0);
			}
		}

		if (HealthComponent)
		{
			HealthComponent->OnHealthChanged.AddDynamic(this, &APlayerCharacter::OnPlayerHealthChanged);
			OnPlayerHealthChanged(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
		}
	}
	SpringArm->SocketOffset.Z = BaseEyeHeight;
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(InputConfig.MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInput->BindAction(InputConfig.JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInput->BindAction(InputConfig.JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInput->BindAction(InputConfig.LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInput->BindAction(InputConfig.FireAction,  ETriggerEvent::Started, this, &APlayerCharacter::StartFire);
		EnhancedInput->BindAction(InputConfig.FireAction,ETriggerEvent::Completed, this, &APlayerCharacter::StopFire);
		EnhancedInput->BindAction(InputConfig.InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Interact);
	}
}

void APlayerCharacter::HandleDeath()
{
	Super::HandleDeath();
	RunMontage(ECharacterAnim::Dead);
	
	UE_LOG(LogTemp, Warning, TEXT("[Player] Player has been killed"));
	OnDeathMontageEnded();
}
void APlayerCharacter::OnDeathMontageEnded()
{
	if (AController* MController = GetController())
	{
		MController->UnPossess();
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

void APlayerCharacter::Interact()
{
}

void APlayerCharacter::OnPlayerHealthChanged(float CurrentHealth, float MaxHealth)
{
	if (UISubsystem)
	{
		UISubsystem->UpdateHealth(CurrentHealth / MaxHealth);
	}
}
