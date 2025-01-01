// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityManager.generated.h"

class UAbilitySettings;

UCLASS()
class GOTA_API UAbilityManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	TArray<UAbilitySettings*> Abilities;
};
