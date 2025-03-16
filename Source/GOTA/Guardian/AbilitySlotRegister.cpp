// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySlotRegister.h"
#include "AbilityFramework/Ability.h"
#include "GOTA/UI/Ingame/AbilitySlot.h"

// ---------------------------------------- AbilitySlots ----------------------------------------

void UAbilitySlotRegister::RegisterAbilitySlotWidget(const FName SlotName, UAbilitySlot* AbilitySlot)
{
	if (FAbilitySlotRegisterEntry* Entry = Register.Find(SlotName))
	{
		// Entry Exists
		Entry->AbilitySlot = AbilitySlot;
		if (Entry->AbilitySlot.IsValid())
		{
			// Inform AbilitySlot of the Ability that was Registered in this SlotName already
			Entry->AbilitySlot->SetAbility(Entry->Ability.Get());
		}
		else if (!Entry->Ability.IsValid())
		{
			// The Entry Exists but both pointers are Invalid. we can remove this Entry
			Register.Remove(SlotName);
		}
	}
	else if (AbilitySlot)
	{
		// Entry doesn't exist and we get a valid AbilitySlot, so we make a new Entry for it
		FAbilitySlotRegisterEntry Value = FAbilitySlotRegisterEntry();
		Value.AbilitySlot = AbilitySlot;
		Value.AbilitySlot->SetAbility(Value.Ability.Get());
		Register.Add(SlotName, Value);
	}
}

void UAbilitySlotRegister::AssignAbilityToSlot(AAbility* Ability, const FName SlotName)
{
	if (SlotName.IsNone()) return;

	if (FAbilitySlotRegisterEntry* Entry = Register.Find(SlotName))
	{
		// Entry Exists
		Entry->Ability = Ability;
		if (Entry->AbilitySlot.IsValid())
		{
			Entry->AbilitySlot->SetAbility(Ability);
		}

		// Entry Exists, but is now empty. we can remove it
		if (!Entry->Ability.IsValid() && !Entry->AbilitySlot.IsValid())
		{
			Register.Remove(SlotName);
		}
	}
	else if (Ability)
	{
		// Entry doesn't Exist and Ability is not nullptr so we need a new Entry
		FAbilitySlotRegisterEntry Value = FAbilitySlotRegisterEntry();
		Value.Ability = Ability;
		if (Value.AbilitySlot.IsValid())
		{
			Value.AbilitySlot->SetAbility(Ability);
		}
		Register.Add(SlotName, Value);
	}
}

void UAbilitySlotRegister::UnassignAbilityFromSlot(const AAbility* Ability, const FName SlotName)
{
	if (SlotName.IsNone()) return;

	if (FAbilitySlotRegisterEntry* Entry = Register.Find(SlotName))
	{
		if (Entry->Ability == Ability)
		{
			Entry->Ability = nullptr;
			if (Entry->AbilitySlot.IsValid())
			{
				Entry->AbilitySlot->SetAbility(nullptr);
			}
		}
	}
}

void UAbilitySlotRegister::SwapAbilitiesInSlots(const FName SlotName1, const FName SlotName2)
{
	AAbility* Ability1 = GetAbilityFromSlotName(SlotName1);
	AssignAbilityToSlot(GetAbilityFromSlotName(SlotName2), SlotName1);
	AssignAbilityToSlot(Ability1, SlotName2);
}

FName UAbilitySlotRegister::GetFreeAbilitySlotName() const
{
	for (const TPair<FName, FAbilitySlotRegisterEntry> Pair : Register)
	{
		if (!Pair.Value.Ability.IsValid() && Pair.Value.AbilitySlot.IsValid())
		{
			return Pair.Key;
		}
	}
	return FName();
}

UAbilitySlot* UAbilitySlotRegister::GetAbilitySlot(const FName SlotName) const
{
	if (const FAbilitySlotRegisterEntry* Entry = Register.Find(SlotName))
	{
		return Entry->AbilitySlot.Get();
	}
	return nullptr;
}

AAbility* UAbilitySlotRegister::GetAbilityFromSlotName(const FName SlotName) const
{
	if (const FAbilitySlotRegisterEntry* Entry = Register.Find(SlotName))
	{
		return Entry->Ability.Get();
	}
	return nullptr;
}
