// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityManager.generated.h"

class UAbilitySlot;
class AAbility;
class UAbilitySettings;

USTRUCT()
struct FAbilitySlotEntry
{
	GENERATED_BODY()
	UPROPERTY(VisibleInstanceOnly)
	TWeakObjectPtr<AAbility> Ability;
	UPROPERTY(VisibleInstanceOnly)
	TWeakObjectPtr<UAbilitySlot> AbilitySlot;
};

UCLASS()
class GOTA_API UAbilityManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	// ---------------------------------------- Lifecycle ----------------------------------------
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	// ---------------------------------------- Utility ----------------------------------------
private:
	TArray<UAbilitySettings*> AbilitySettings;
	
public:
	TArray<UAbilitySettings*> GetAllAbilitySettings() const { return AbilitySettings; }
	
	// ---------------------------------------- AbilitySlots ----------------------------------------
	// pure clientside register for AbilitySlot widgets to somehow create a connection between the Ability and
	// the UI. im not sure if this is a good idea but i also don't know how else we could solve this
	
private:
	UPROPERTY(VisibleInstanceOnly, Category = "AbilityManager")
	TMap<FName, FAbilitySlotEntry> AbilitySlotRegister;
	
	void SpawnAbilityWidgetIfNeeded(FAbilitySlotEntry* Entry);
	
public:
	void RegisterAbilitySlot(FName SlotName, UAbilitySlot* AbilitySlot);
	void UnregisterAbilitySlot(FName SlotName, UAbilitySlot* AbilitySlot);
	
	void RegisterAbilityInSlot(FName SlotName, AAbility* Ability);
	void UnregisterAbilityInSlot(FName SlotName, AAbility* Ability);
	
	FName GetFreeAbilitySlotName() const;
	UAbilitySlot* GetAbilitySlot(FName SlotName) const;
	AAbility* GetAbilityFromSlotName(FName SlotName);
};
