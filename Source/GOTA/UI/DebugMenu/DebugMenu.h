#pragma once

#include "Blueprint/UserWidget.h"
#include "DebugMenu.generated.h"

class UButton;
class UWidgetSwitcher;

UCLASS()
class UDebugMenu : public UUserWidget
{
	GENERATED_BODY()

	// -------------------------------------- LifeCycle --------------------------------------

	virtual void NativeConstruct() override;

	// ---------------------------------------- Utility ----------------------------------------
private:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_OpenTimeControls;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_OpenAbilityGranter;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_OpenGotaRLMenu;

	UFUNCTION()
	void OpenTimeControls();

	UFUNCTION()
	void OpenAbilityGranter();

	UFUNCTION()
	void OpenGotaRLMenu();

	// ------------------------------------ Prevent Clicking Through ------------------------------------

	virtual FReply NativeOnMouseButtonDown
	(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDoubleClick
	(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
