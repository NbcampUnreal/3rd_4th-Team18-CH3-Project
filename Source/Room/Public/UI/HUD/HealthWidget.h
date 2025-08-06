#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

class UProgressBar;
class UImage;

UCLASS()
class ROOM_API UHealthWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget)) UProgressBar* HealthBar;

    // 추후 블러드 스크린용 이미지 (현재는 비활성화 상태)
    UPROPERTY(meta = (BindWidgetOptional)) UImage* BloodScreen;

public:
    void UpdateHealth(float HealthRatio); // 0.0 ~ 1.0
};