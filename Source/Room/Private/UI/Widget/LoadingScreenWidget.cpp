#include "UI/Widget/LoadingScreenWidget.h"
#include "Subsystem/LoadingSubsystem.h"
#include "Engine/GameInstance.h"
#include "Components/ProgressBar.h"

void ULoadingScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!ProgressBar)
	{
		return;
	}

	const float LoadProgress = GetProgress01();
	ProgressBar->SetPercent(LoadProgress);
}

float ULoadingScreenWidget::GetProgress01() const
{
	if (const UGameInstance* GI = GetGameInstance())
	{
		if (const ULoadingSubsystem* Loader = GI->GetSubsystem<ULoadingSubsystem>())
		{
			const float Value = Loader->GetLoadingProgress();
			if (Value < 0.f)
			{
				return 0.f;
			}
			return FMath::Clamp(Value, 0.f, 1.f);
		}
	}

	return 0.f;
}

