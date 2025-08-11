#include "UI/Widget/DamageTextActor.h"

#include "Components/WidgetComponent.h"

#include "UI/Widget/DamageTextWidget.h"

ADamageTextActor::ADamageTextActor()
{
    PrimaryActorTick.bCanEverTick = false;

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    SetRootComponent(WidgetComponent);

    WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

    InitialLifeSpan = 1.5f;
}

void ADamageTextActor::SetDamage(int32 Damage)
{
    UDamageTextWidget* DamageWidget = Cast<UDamageTextWidget>(WidgetComponent->GetUserWidgetObject());
    if (DamageWidget)
    {
        DamageWidget->SetDamageText(Damage);
    }
}