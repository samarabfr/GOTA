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

void UAbilityManager::SpawnAbilityWidgetIfNeeded(FAbilitySlotEntry* Entry)
{
	if (Entry && Entry->Ability.IsValid() && Entry->AbilitySlot.IsValid())
	{
		Entry->AbilitySlot->SpawnAbilityWidget(Entry->Ability.Get());
	}
}

void UAbilityManager::RegisterAbilitySlot(FName SlotName, UAbilitySlot* AbilitySlot)
{
	if (FAbilitySlotEntry* Entry = AbilitySlotRegister.Find(SlotName))
	{
		if (Entry->AbilitySlot.IsValid())
		{
			UE_LOG(LogTemp, Warning,
			       TEXT("Tried to Register an AbilitySlot that already exists. Slot: %s")
			       , *SlotName.ToString())
		}
		else
		{
			Entry->AbilitySlot = AbilitySlot;
			SpawnAbilityWidgetIfNeeded(Entry);
		}
	}
	else
	{
		FAbilitySlotEntry Value = FAbilitySlotEntry();
		Value.AbilitySlot = AbilitySlot;
		AbilitySlotRegister.Add(SlotName, Value);
	}
}

void UAbilityManager::UnregisterAbilitySlot(FName SlotName, UAbilitySlot* AbilitySlot)
{
	if (FAbilitySlotEntry* Entry = AbilitySlotRegister.Find(SlotName))
	{
		Entry->AbilitySlot.Reset();
		if (!Entry->Ability.IsValid())
		{
			AbilitySlotRegister.Remove(SlotName);
		}
	}
}

void UAbilityManager::RegisterAbilityInSlot(FName SlotName, AAbility* Ability)
{
	if (FAbilitySlotEntry* Entry = AbilitySlotRegister.Find(SlotName))
	{
		if (Entry->Ability.IsValid())
		{
			UE_LOG(LogTemp, Warning,
			       TEXT("Tried to Register an Ability to an AbilitySlot that is already in use. Slot: %s")
			       , *SlotName.ToString())
		}
		else
		{
			Entry->Ability = Ability;
			SpawnAbilityWidgetIfNeeded(Entry);
		}
	}
	else
	{
		FAbilitySlotEntry Value = FAbilitySlotEntry();
		Value.Ability = Ability;
		AbilitySlotRegister.Add(SlotName, Value);
	}
}

void UAbilityManager::UnregisterAbilityInSlot(const FName SlotName, AAbility* Ability)
{
	if (FAbilitySlotEntry* Entry = AbilitySlotRegister.Find(SlotName))
	{
		Entry->Ability.Reset();
		if (!Entry->AbilitySlot.IsValid())
		{
			AbilitySlotRegister.Remove(SlotName);
		}
	}
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

UAbilitySlot* UAbilityManager::GetAbilitySlot(FName SlotName) const
{
	if (const FAbilitySlotEntry* Entry = AbilitySlotRegister.Find(SlotName))
	{
		return Entry->AbilitySlot.Get();
	}
	return nullptr;
}

AAbility* UAbilityManager::GetAbilityFromSlotName(FName SlotName)
{
	if (const FAbilitySlotEntry* Entry = AbilitySlotRegister.Find(SlotName))
	{
		return Entry->Ability.Get();
	}
	return nullptr;
}
