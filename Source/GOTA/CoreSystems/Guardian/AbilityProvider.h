// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityProvider.generated.h"

class AAbility;
class UAbilitySettings;

UCLASS()
class GOTA_API UAbilityProvider : public UGameInstanceSubsystem
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
	
};
