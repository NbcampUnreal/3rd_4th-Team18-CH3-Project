#include "UI/HUD/HealthWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HealthWidget)

void UHealthWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (HealthBar)
    {
        HealthBar->SetPercent(1.0f); // ü�� 100% �⺻��
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

    // ���� ��ũ�� �ð� ȿ���� ü���� 0.3 ������ ���� ���̰�
    if (BloodScreen)
    {
        const bool bLowHealth = HealthRatio <= 0.3f;
        BloodScreen->SetVisibility(bLowHealth ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}