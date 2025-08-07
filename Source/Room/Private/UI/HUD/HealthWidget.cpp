#include "UI/HUD/HealthWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HealthWidget)

void UHealthWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (HealthBar)
    {
        HealthBar->SetPercent(1.0f); // 체력 100% 기본값
    }

    if (BloodScreen)
    {
        BloodScreen->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UHealthWidget::UpdateHealth(float HealthRatio)
{
    if (HealthBar)
    {
        HealthBar->SetPercent(HealthRatio);
    }

    // 블러드 스크린 시각 효과는 체력이 0.3 이하일 때만 보이게
    if (BloodScreen)
    {
        const bool bLowHealth = HealthRatio <= 0.3f;
        BloodScreen->SetVisibility(bLowHealth ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}