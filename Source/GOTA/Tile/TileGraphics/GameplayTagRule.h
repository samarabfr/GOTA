// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "GameplayTagRule.generated.h"

UENUM(BlueprintType)
enum class ERuleMode : uint8
{
	// true if one GameplayTag match
	MatchAny UMETA(DisplayName = "Match Any"),
	// true if all GameplayTag match
	MatchAll UMETA(DisplayName = "Match All"),
	// false if any GameplayTag match
	ForbidAny UMETA(DisplayName = "Forbid Any"),
	// false if this combination of Tags matches
	ForbidExactly UMETA(DisplayName = "Forbid Exactly")
};

USTRUCT(BlueprintType)
struct FGameplayTagRule
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GameplayTag Rule")
	ERuleMode TagMode = ERuleMode::MatchAny;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GameplayTag Rule")
	FGameplayTagContainer ConditionTags;

	bool IsValid(const FGameplayTagContainer& GameplayTagContainer) const;
};
