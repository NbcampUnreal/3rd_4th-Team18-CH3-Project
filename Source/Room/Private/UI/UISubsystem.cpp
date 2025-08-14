#include "UI/UISubsystem.h"
#include "UI/Widget/MainMenuWidget.h"
#include "UI/Widget/PauseMenuWidget.h"
#include "UI/Widget/HUDWidget.h"
#include "UI/Widget/LoadingScreenWidget.h"
#include "UI/Widget/DamageTextActor.h"
#include "ItemSystem/UI/InventoryWidget/InventoryWidget.h"

#include "Blueprint/UserWidget.h"
#include "Core/GameManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

#include "Subsystem/StaticDataSubsystem.h"
#include "StaticData/StaticDataStruct.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UISubsystem)


void UUISubsystem::Initialize(FSubsystemCollectionBase & Collection)
{
    Super::Initialize(Collection);
    //auto Data = Collection.InitializeDependency<UStaticDataSubsystem>();
}

void UUISubsystem::InitUIResources()
{
    UGameInstance* GameInstance = GetGameInstance();
    if (!GameInstance) return;

    UStaticDataSubsystem* StaticDataSubsystem = GetGameManager()->GetSubsystem<UStaticDataSubsystem>();
    check(StaticDataSubsystem);
    if (!StaticDataSubsystem) return;

    const FUIData* UIData = StaticDataSubsystem->GetData<FUIData>(UIDataID);
    if (!UIData) return;

    MainMenuWidgetClass = UIData->MainMenuWidgetClass.LoadSynchronous();
    PauseMenuWidgetClass = UIData->PauseMenuWidgetClass.LoadSynchronous();
    HUDWidgetClass = UIData->HUDWidgetClass.LoadSynchronous();
    LoadingWidgetClass = UIData->LoadingScreenWidgetClass.LoadSynchronous();
    DamageTextActorClass = UIData->DamageTextActorClass.LoadSynchronous();
    InventoryWidgetClass = UIData->InventoryWidgetClass.LoadSynchronous();

    if (MainMenuWidgetClass)
    {
        UE_LOG(LogTemp, Log, TEXT("SUCCESS: MainMenu Class Loaded from DataTable."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("FATAL ERROR: MainMenu Class FAILED to load from DataTable. Check Path in DT_UIDataTable!"));
    }
}

void UUISubsystem::Deinitialize()
{
    MainMenuWidget.Reset();
    PauseMenuWidget.Reset();
    HUDWidget.Reset();
    InventoryWidget.Reset();
}

void UUISubsystem::SetUIInputMode()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        FInputModeUIOnly InputMode{};
        InputMode.SetWidgetToFocus(nullptr);
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

        PC->SetInputMode(InputMode);

        PC->bShowMouseCursor = true;
        PC->bEnableClickEvents = true;
        PC->bEnableMouseOverEvents = true;

        PC->SetIgnoreLookInput(true);
        PC->SetIgnoreMoveInput(true);
    }
}

void UUISubsystem::SetGameInputMode()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->SetInputMode(FInputModeGameOnly());

        PC->bShowMouseCursor = false;
        PC->bEnableClickEvents = false;
        PC->bEnableMouseOverEvents = false;

        PC->SetIgnoreLookInput(false);
        PC->SetIgnoreMoveInput(false);
    }
}

void UUISubsystem::ShowMainMenu()
{
    if (!MainMenuWidget.IsValid() && MainMenuWidgetClass)
    {
        MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
    }

    if (MainMenuWidget.IsValid())
    {
        MainMenuWidget->AddToViewport();
        SetUIInputMode();
    }
}

void UUISubsystem::ShowPauseMenu()
{
    if (!PauseMenuWidget.IsValid() && PauseMenuWidgetClass)
    {
        PauseMenuWidget = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseMenuWidgetClass);
    }

    if (PauseMenuWidget.IsValid())
    {
        PauseMenuWidget->AddToViewport();
        SetUIInputMode();
    }

    HideHUD();
    UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UUISubsystem::HidePauseMenu()
{
    if (PauseMenuWidget.IsValid())
    {
        PauseMenuWidget->RemoveFromParent();
    }

    ShowHUD();
    UGameplayStatics::SetGamePaused(GetWorld(), false);
    SetGameInputMode();
}

void UUISubsystem::ShowHUD()
{
    if (!HUDWidget.IsValid() && HUDWidgetClass)
    {
        HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), HUDWidgetClass);
    }

    if (HUDWidget.IsValid())
        HUDWidget->AddToViewport();
    SetGameInputMode();
}

void UUISubsystem::HideHUD()
{
    if (HUDWidget.IsValid())
        HUDWidget->RemoveFromParent();
}

void UUISubsystem::UpdateWeaponInfo(UTexture2D* Icon, FName Name, int32 AmmoCount)
{
    if (HUDWidget.IsValid())
    {
        HUDWidget->UpdateWeaponAndAmmo(Icon, Name, AmmoCount);
    }
}

void UUISubsystem::UpdateObjective(int32 RangedKill, int32 RangedTotal, int32 MeleeKill, int32 MeleeTotal)
{
    if (HUDWidget.IsValid())
        HUDWidget->UpdateObjective(RangedKill, RangedTotal, MeleeKill, MeleeTotal);
}

void UUISubsystem::UpdateHealth(float HealthRatio)
{
    if (HUDWidget.IsValid())
    {
        HUDWidget->UpdateHealth(HealthRatio);
    }
}

void UUISubsystem::ShowLoadingScreen()
{
    if (!LoadingWidget.IsValid() && LoadingWidgetClass)
    {
        LoadingWidget = CreateWidget<ULoadingScreenWidget>(GetWorld(), LoadingWidgetClass);
    }

    if (LoadingWidget.IsValid())
    {
        LoadingWidget->AddToViewport();
        SetUIInputMode();
    }
}

void UUISubsystem::HideLoadingScreen()
{
    if (LoadingWidget.IsValid())
    {
        LoadingWidget->RemoveFromParent();
    }
}

void UUISubsystem::ShowHitMarkerOnHUD()
{
    if (HUDWidget.IsValid())
    {
        HUDWidget->ShowCrosshairHitMarker();
    }
}

void UUISubsystem::ShowKillMarkerOnHUD()
{
    if (HUDWidget.IsValid())
    {
        HUDWidget->ShowKillMarker();
    }
}

void UUISubsystem::ShowDamageNumber(int32 Damage, FVector WorldLocation)
{
    if (GetWorld() && DamageTextActorClass)
    {
        ADamageTextActor* SpawnedActor = GetWorld()->SpawnActor<ADamageTextActor>(DamageTextActorClass, WorldLocation, FRotator::ZeroRotator);
        if (SpawnedActor)
        {
            SpawnedActor->SetDamage(Damage);
        }
    }
}

void UUISubsystem::ToggleInventory()
{
    if (!HUDWidget.IsValid())
    {
        return;
    }

    const bool bOpen = HUDWidget->IsInventoryVisible();

    if (bOpen)
    {
        HUDWidget->SetInventoryVisible(false);
        SetGameInputMode();
    }
    else
    {
        HUDWidget->SetInventoryVisible(true);
        SetUIInputMode();
    }
}

void UUISubsystem::ShowGuideMessage(const FText& Message)
{
    if (HUDWidget.IsValid())
    {
        HUDWidget->ShowGuideMessage(Message);
    }
}

void UUISubsystem::ShowInteractMessage(const FText& Message)
{
    if (HUDWidget.IsValid())
    {
        HUDWidget->ShowInteractMessage(Message);
    }
}

void UUISubsystem::HideInteractMessage()
{
    if (HUDWidget.IsValid())
    {
        HUDWidget->HideInteractMessage();
    }
}

void UUISubsystem::UpdateScore(int32 NewScore)
{
    if (HUDWidget.IsValid())
    {
        HUDWidget->UpdateScore(NewScore);
    }
}
bool UUISubsystem::IsInventoryOpen() const
{
    return HUDWidget.IsValid() && HUDWidget->IsInventoryVisible();
}