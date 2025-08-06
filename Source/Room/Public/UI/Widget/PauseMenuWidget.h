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

    // 바인딩 요소
    UPROPERTY(meta = (BindWidget)) UButton* ResumeButton;
    UPROPERTY(meta = (BindWidget)) UButton* MainMenuButton;
    UPROPERTY(meta = (BindWidget)) UButton* ExitButton;

    UPROPERTY(meta = (BindWidget)) UTextBlock* DescriptionText;

    // 클릭 이벤트
    UFUNCTION() void OnResumeClicked();
    UFUNCTION() void OnMainMenuClicked();
    UFUNCTION() void OnExitClicked();

    // 호버 이벤트
    UFUNCTION() void OnResumeHovered();
    UFUNCTION() void OnMainMenuHovered();
    UFUNCTION() void OnExitHovered();
    UFUNCTION() void OnButtonUnhovered();
};