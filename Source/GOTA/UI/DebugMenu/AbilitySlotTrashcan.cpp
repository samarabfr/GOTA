#include "AbilitySlotTrashcan.h"

#include "GOTA/CoreSystems/Guardian/Ability.h"
#include "GOTA/CoreSystems/Guardian/AbilitySlotRegister.h"

void UAbilitySlotTrashcan::OnSuccessfulDrop(UAbilitySlot* OriginSlot)
{
	AAbility* OriginAbility = OriginSlot->GetAbility();

	// Set Ability in the OriginSlot to nullptr
	UAbilitySlotRegister* AbilityManager = GetGameInstance()->GetSubsystem<UAbilitySlotRegister>();
	if (AbilityManager)
	{
		AbilityManager->AssignAbilityToSlot(nullptr, OriginSlot->GetSlotName());
	}

	// Destroy the Ability
	if (OriginAbility)
	{
		OriginAbility->Destroy();
	}
}
