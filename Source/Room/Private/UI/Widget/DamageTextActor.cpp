#include "UI/Widget/DamageTextActor.h"
#include "Components/WidgetComponent.h"
#include "UI/HUD/DamageNumberWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ADamageTextActor::ADamageTextActor()
{
    PrimaryActorTick.bCanEverTick = false;

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    WidgetComponent->SetupAttachment(RootComponent);
    WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // 또는 World
    WidgetComponent->SetDrawSize(FVector2D(100.f, 50.f));
    WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADamageTextActor::BeginPlay()
{
    Super::BeginPlay();

    // 일정 시간 후 제거
    GetWorldTimerManager().SetTimer(DestroyTimer, this, &ADamageTextActor::DestroySelf, DestroyDelay, false);
}

void ADamageTextActor::DestroySelf()
{
    Destroy();
}

void ADamageTextActor::SetDamageValue(int32 Damage)
{
    if (UUserWidget* Widget = WidgetComponent->GetUserWidgetObject())
    {
        if (UDamageNumberWidget* DamageWidget = Cast<UDamageNumberWidget>(Widget))
        {
            DamageWidget->SetDamage(Damage);
        }
    }
}