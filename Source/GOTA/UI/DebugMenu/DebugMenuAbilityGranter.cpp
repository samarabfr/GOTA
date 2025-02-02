#include "DebugMenuAbilityGranter.h"

#include "DebugMenuAbilityEntry.h"
#include "Components/UniformGridPanel.h"
#include "GOTA/CoreSystems/Guardian/AbilityProvider.h"

// -------------------------------------------- LifeCycle --------------------------------------------

void UDebugMenuAbilityGranter::NativeConstruct()
{
	Super::NativeConstruct();
	if (const UAbilityProvider* AbilityProvider = GetGameInstance()->GetSubsystem<UAbilityProvider>())
	{
		for (UAbilitySettings* AbilitySettings : AbilityProvider->GetAllAbilitySettings())
		{
			AddToGrid(AbilitySettings);
		}
	}
}

// -------------------------------------------- Utility --------------------------------------------

void UDebugMenuAbilityGranter::AddToGrid(UAbilitySettings* AbilitySettings)
{
	if (!AbilityEntryClass || !AbilitySettings) return;

	UDebugMenuAbilityEntry* AbilityEntry = CreateWidget<UDebugMenuAbilityEntry>(this, AbilityEntryClass);
	const int32 Column = FMath::Modulo(GridCounter, GridColumns);
	const int32 Row = GridCounter / GridColumns;
	AbilityEntry->SetAbility(AbilitySettings);
	GrantAbilityGrid->AddChildToUniformGrid(AbilityEntry, Row, Column);
	++GridCounter;
}
