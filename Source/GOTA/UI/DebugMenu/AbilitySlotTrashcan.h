#pragma once

#include "GOTA/UI/Ingame/AbilitySlot.h"
#include "AbilitySlotTrashcan.generated.h"

UCLASS(Blueprintable)
class UAbilitySlotTrashcan : public UAbilitySlot
{
	GENERATED_BODY()

	// ---------------------------------------- Utility ----------------------------------------
	
	virtual void OnSuccessfulDrop(UAbilitySlot* OriginSlot) override;
};
