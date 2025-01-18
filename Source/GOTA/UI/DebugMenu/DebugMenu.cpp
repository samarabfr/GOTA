#include "DebugMenu.h"

#include "DebugMenuAbilityEntry.h"
#include "Components/UniformGridPanel.h"
#include "GOTA/CoreSystems/Guardian/AbilityProvider.h"
#include "GOTA/CoreSystems/Guardian/AbilitySlotRegister.h"

void UDebugMenu::NativeConstruct()
{
	Super::NativeConstruct();
	const UAbilityProvider* AbilityProvider = GetGameInstance()->GetSubsystem<UAbilityProvider>();
	if (AbilityProvider)
	{
		for (int32 i = 0; i < 10; ++i)
		{
			for (UAbilitySettings* AbilitySettings : AbilityProvider->GetAllAbilitySettings())
			{
				AddToGrid(AbilitySettings);
			}
		}
	}
}



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