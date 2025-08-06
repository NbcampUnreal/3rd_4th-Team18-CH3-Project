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

    // ���� ���� ��ũ���� �̹��� (����� ��Ȱ��ȭ ����)
    UPROPERTY(meta = (BindWidgetOptional)) UImage* BloodScreen;

public:
    void UpdateHealth(float HealthRatio); // 0.0 ~ 1.0
};