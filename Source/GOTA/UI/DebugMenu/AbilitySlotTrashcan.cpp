#include "AbilitySlotTrashcan.h"

#include "GOTA/CoreSystems/Guardian/Ability.h"

void UAbilitySlotTrashcan::OnSuccessfulDrop(UAbilitySlot* OriginSlot)
{
	AAbility* OriginAbility = OriginSlot->GetAbility();
	OriginSlot->SetAbility(nullptr);
	OriginAbility->Destroy();
}
