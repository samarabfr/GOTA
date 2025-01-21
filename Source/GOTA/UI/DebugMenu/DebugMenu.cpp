#include "DebugMenu.h"

#include "DebugMenuAbilityEntry.h"
#include "Components/UniformGridPanel.h"
#include "GOTA/CoreSystems/Guardian/AbilityProvider.h"

// -------------------------------------- LifeCycle --------------------------------------

void UDebugMenu::NativeConstruct()
{
	Super::NativeConstruct();
	const UAbilityProvider* AbilityProvider = GetGameInstance()->GetSubsystem<UAbilityProvider>();
	if (AbilityProvider)
	{
		for (UAbilitySettings* AbilitySettings : AbilityProvider->GetAllAbilitySettings())
		{
			AddToGrid(AbilitySettings);
		}
	}
}

// ---------------------------------------- Utility ----------------------------------------

void UDebugMenu::AddToGrid(UAbilitySettings* AbilitySettings)
{
	if (!AbilityEntryClass || !GrantAbilityGrid || !AbilitySettings) return;

	UDebugMenuAbilityEntry* AbilityEntry = CreateWidget<UDebugMenuAbilityEntry>(this, AbilityEntryClass);
	const int32 Column = FMath::Modulo(GridCounter, GridColumns);
	const int32 Row = GridCounter / GridColumns;
	AbilityEntry->SetAbility(AbilitySettings);
	GrantAbilityGrid->AddChildToUniformGrid(AbilityEntry, Row, Column);
	++GridCounter;
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
