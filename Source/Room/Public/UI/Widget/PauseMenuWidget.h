#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class ROOM_API UPauseMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    // ���ε� ���
    UPROPERTY(meta = (BindWidget)) UButton* ResumeButton;
    UPROPERTY(meta = (BindWidget)) UButton* MainMenuButton;
    UPROPERTY(meta = (BindWidget)) UButton* ExitButton;

    UPROPERTY(meta = (BindWidget)) UTextBlock* DescriptionText;

    // Ŭ�� �̺�Ʈ
    UFUNCTION() void OnResumeClicked();
    UFUNCTION() void OnMainMenuClicked();
    UFUNCTION() void OnExitClicked();

    // ȣ�� �̺�Ʈ
    UFUNCTION() void OnResumeHovered();
    UFUNCTION() void OnMainMenuHovered();
    UFUNCTION() void OnExitHovered();
    UFUNCTION() void OnButtonUnhovered();
};