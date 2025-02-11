#include "DebugMenu.h"

#include "DebugMenuAbilityEntry.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

// -------------------------------------- LifeCycle --------------------------------------

void UDebugMenu::NativeConstruct()
{
	Super::NativeConstruct();

	BTN_OpenTimeControls->OnClicked.AddDynamic(this, &UDebugMenu::OpenTimeControls);
	BTN_OpenAbilityGranter->OnClicked.AddDynamic(this, &UDebugMenu::OpenAbilityGranter);
	BTN_OpenGotaRLMenu->OnClicked.AddDynamic(this, &UDebugMenu::OpenGotaRLMenu);
}

// ---------------------------------------- Utility ----------------------------------------

void UDebugMenu::OpenTimeControls()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}

void UDebugMenu::OpenAbilityGranter()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

void UDebugMenu::OpenGotaRLMenu()
{
	WidgetSwitcher->SetActiveWidgetIndex(2);
}

// ------------------------------------ Prevent Clicking Through ------------------------------------

FReply UDebugMenu::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply UDebugMenu::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}
