#include "Actor/Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ItemSystem/InteractionComponent/InteractionComponent.h"
#include "StaticData/StatData.h"
#include "Subsystem/StaticDataSubsystem.h"
#include "UI/UISubsystem.h"

APlayerCharacter::APlayerCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SocketOffset = FVector(0.0f, 50.0f, 0.0f);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	WeaponComponent->SetupAttachment(GetMesh(), FName("Muzzle"));

	bUseControllerRotationYaw = true;
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = Speed;
	
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	OwnedGameplayTags.AddTag(GameDefine::PlayerTag);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UISubsystem = GameInstance->GetSubsystem<UUISubsystem>();
	}

	UStaticDataSubsystem* StaticDataSubsystem = GetGameInstance()->GetSubsystem<UStaticDataSubsystem>();
	if (!StaticDataSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PlayerCharacter] StaticDataSubsystem not found"));
		return;
	}

	// 
	const FStatData* PlayerStat = StaticDataSubsystem->GetData<FStatData>(PlayerStatID);
	if (!PlayerStat)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PlayerCharacter] PlayerStatID %d not found in DT_StatDataTable"), PlayerStatID);
		return;
	}

	InitializeFromStaticData(PlayerStat);


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
		EnhancedInput->BindAction(InputConfig.FireAction, ETriggerEvent::Started, this, &APlayerCharacter::StartFire);
		EnhancedInput->BindAction(InputConfig.FireAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopFire);
		EnhancedInput->BindAction(InputConfig.InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Interact);
		EnhancedInput->BindAction(InputConfig.InventoryToggle, ETriggerEvent::Triggered, this, &APlayerCharacter::InventoryToggle);
	}
}

void APlayerCharacter::HandleDeath()
{
	Super::HandleDeath();
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
	if (WeaponComponent)
	{
		WeaponComponent->Fire();
		RunMontage(ECharacterAnim::Attacking);
	}
}
void APlayerCharacter::StopFire()
{
	StopMontage();
}

void APlayerCharacter::Interact()
{
	InteractionComponent->Interact();
}

void APlayerCharacter::InventoryToggle()
{
	if (!UISubsystem) return;

	UISubsystem->ToggleInventory();
}

void APlayerCharacter::InitializeFromStaticData(const FStaticData* InStaticData)
{
	if (!InStaticData) return;

	const FStatData* StatData = static_cast<const FStatData*>(InStaticData);
	if (!StatData)
	{
		UE_LOG(LogTemp, Error, TEXT("[PlayerCharacter] Failed to cast FStaticData to FStatData"));
		return;
	}
	
	if (HealthComponent)
	{
		HealthComponent->SetMaxHealth(StatData->HP);
		HealthComponent->SetCurrentHealth(StatData->HP);
	}
	Attack = StatData->Attack;
	Defense = StatData->Defense;

	UE_LOG(LogTemp, Log, TEXT("[PlayerCharacter] Stats initialized: HP=%f, Attack=%f, Defense=%f"),
		   StatData->HP, StatData->Attack, StatData->Defense);
}

void APlayerCharacter::OnPlayerHealthChanged(float CurrentHealth, float MaxHealth)
{
	if (UISubsystem)
	{
		UISubsystem->UpdateHealth(CurrentHealth / MaxHealth);
	}
}
