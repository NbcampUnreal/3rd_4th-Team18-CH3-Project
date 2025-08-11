// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSystem/TestFramework/ItemPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ItemSystem/Item/WeaponItem/WeaponItem.h"


// Sets default values
AItemPlayerCharacter::AItemPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

}


