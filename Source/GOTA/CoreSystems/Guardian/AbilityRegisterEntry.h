// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityRegisterEntry.generated.h"

class UAbilitySettings;

USTRUCT(BlueprintType)
struct FAbilityRegisterEntry : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	UAbilitySettings* AbilitySettings = nullptr;
};
