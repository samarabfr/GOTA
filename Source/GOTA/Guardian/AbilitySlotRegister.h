// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySlotRegister.generated.h"

class UAbilitySlot;
class AAbility;
class UAbilitySettings;

USTRUCT()
struct FAbilitySlotRegisterEntry
{
	GENERATED_BODY()
	UPROPERTY(VisibleInstanceOnly)
	TWeakObjectPtr<AAbility> Ability;
	UPROPERTY(VisibleInstanceOnly)
	TWeakObjectPtr<UAbilitySlot> AbilitySlot;
};

UCLASS()
class GOTA_API UAbilitySlotRegister : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleInstanceOnly, Category = "AbilityManager")
	TMap<FName, FAbilitySlotRegisterEntry> Register;

public:
	void RegisterAbilitySlotWidget(const FName SlotName, UAbilitySlot* AbilitySlot);
	
	void AssignAbilityToSlot(AAbility* Ability, const FName SlotName);
	void UnassignAbilityFromSlot(const AAbility* Ability, const FName SlotName);
	
	void SwapAbilitiesInSlots(const FName SlotName1, const FName SlotName2);

	FName GetFreeAbilitySlotName() const;
	UAbilitySlot* GetAbilitySlot(const FName SlotName) const;
	AAbility* GetAbilityFromSlotName(const FName SlotName) const;
};
