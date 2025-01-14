// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityManager.h"
#include "AbilityRegisterEntry.h"
#include "Ability.h"
#include "GOTA/UI/Ingame/AbilitySlot.h"

// ---------------------------------------- Utility ----------------------------------------

void UAbilityManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	static const TCHAR* PathToDataTable = TEXT("/Game/CoreSystems/Guardian/Abilities/DT_AbilityRegister");
	if (UDataTable* AbilityRegister = LoadObject<UDataTable>(nullptr, PathToDataTable))
	{
		AbilityRegister->ForeachRow<FAbilityRegisterEntry>(
			TEXT("Load Abilities"),
			[&](const FName& RowName, const FAbilityRegisterEntry& RowData)
			{
				AbilitySettings.Add(RowData.AbilitySettings);
			});
	}
	UE_LOG(LogTemp, Warning, TEXT("Loaded %d Abilities"), AbilitySettings.Num())
}

// ---------------------------------------- AbilitySlots ----------------------------------------

void UAbilityManager::RegisterAbilitySlotWidget(const FName SlotName, UAbilitySlot* AbilitySlot)
{
	if (FAbilitySlotEntry* Entry = AbilitySlotRegister.Find(SlotName))
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
			AbilitySlotRegister.Remove(SlotName);
		}
	}
	else if (AbilitySlot)
	{
		// Entry doesn't exist and we get a valid AbilitySlot, so we make a new Entry for it
		FAbilitySlotEntry Value = FAbilitySlotEntry();
		Value.AbilitySlot = AbilitySlot;
		Value.AbilitySlot->SetAbility(Value.Ability.Get());
		AbilitySlotRegister.Add(SlotName, Value);
	}
}

void UAbilityManager::AssignAbilityToSlot(const FName SlotName, AAbility* Ability)
{
	if (FAbilitySlotEntry* Entry = AbilitySlotRegister.Find(SlotName))
	{
		// Entry Exists
		Entry->Ability = Ability;
		if (Entry->AbilitySlot.IsValid())
			Entry->AbilitySlot->SetAbility(Entry->Ability.Get());

		// Entry Exists, but is now empty. we can remove it
		if (!Entry->Ability.IsValid() && !Entry->AbilitySlot.IsValid())
		{
			AbilitySlotRegister.Remove(SlotName);
		}
	}
	else if (Ability)
	{
		// Entry doesn't Exist and Ability is not nullptr so we need a new Entry
		FAbilitySlotEntry Value = FAbilitySlotEntry();
		Value.Ability = Ability;
		if (Value.AbilitySlot.IsValid())
			Value.AbilitySlot->SetAbility(Value.Ability.Get());
		AbilitySlotRegister.Add(SlotName, Value);
	}
}

void UAbilityManager::SwapAbilitiesInSlots(const FName SlotName1, const FName SlotName2)
{
	AAbility* Ability1 = GetAbilityFromSlotName(SlotName1);
	AssignAbilityToSlot(SlotName1, GetAbilityFromSlotName(SlotName2));
	AssignAbilityToSlot(SlotName2, Ability1);
}

FName UAbilityManager::GetFreeAbilitySlotName() const
{
	for (const TPair<FName, FAbilitySlotEntry> Pair : AbilitySlotRegister)
	{
		if (!Pair.Value.Ability.IsValid() && Pair.Value.AbilitySlot.IsValid())
		{
			return Pair.Key;
		}
	}
	return FName();
}

UAbilitySlot* UAbilityManager::GetAbilitySlot(const FName SlotName) const
{
	if (const FAbilitySlotEntry* Entry = AbilitySlotRegister.Find(SlotName))
	{
		return Entry->AbilitySlot.Get();
	}
	return nullptr;
}

AAbility* UAbilityManager::GetAbilityFromSlotName(const FName SlotName) const
{
	if (const FAbilitySlotEntry* Entry = AbilitySlotRegister.Find(SlotName))
	{
		return Entry->Ability.Get();
	}
	return nullptr;
}
