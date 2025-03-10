// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnLayoutStruct.h"

bool FSpawnLayoutStruct::IsValidFor(const FGameplayTagContainer& GameplayTagContainer)
{
	for (FGameplayTagRule Rule : GameplayTagRules)
	{
		if (!Rule.IsValid(GameplayTagContainer)) return false;
	}
	return true;
}
