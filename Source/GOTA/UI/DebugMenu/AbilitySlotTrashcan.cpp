#include "AbilitySlotTrashcan.h"

#include "GOTA/CoreSystems/Guardian/Ability.h"
#include "GOTA/CoreSystems/Guardian/AbilityManager.h"

void UAbilitySlotTrashcan::OnSuccessfulDrop(UAbilitySlot* OriginSlot)
{
	AAbility* OriginAbility = OriginSlot->GetAbility();

	// Set Ability in the OriginSlot to nullptr
	UAbilityManager* AbilityManager = GetGameInstance()->GetSubsystem<UAbilityManager>();
	if (AbilityManager)
	{
		AbilityManager->AssignAbilityToSlot(OriginSlot->GetSlotName(), nullptr);
	}

	// Destroy the Ability
	if (OriginAbility)
	{
		OriginAbility->Destroy();
	}
}
