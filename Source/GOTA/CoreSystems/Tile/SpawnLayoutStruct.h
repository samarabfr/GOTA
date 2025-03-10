// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnLayout.h"
#include "UObject/Object.h"
#include "SpawnLayoutStruct.generated.h"

USTRUCT()
struct GOTA_API FSpawnLayoutStruct
{
	GENERATED_BODY()

	FName Name;

	TArray<FGameplayTagRule> GameplayTagRules;

	bool GuaranteedIfPossible = false;

	FSpawnBias SpawnBias;

	FSpawnLayout SpawnLayout;

	bool IsValidFor(const FGameplayTagContainer& GameplayTagContainer);
};
