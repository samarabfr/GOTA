// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "TileAsset.generated.h"

UENUM(BlueprintType)
enum class EGameplayTagMode : uint8
{
	MatchAny UMETA(DisplayName = "Match Any"),
	MatchAll UMETA(DisplayName = "Match All")
};

USTRUCT(BlueprintType)
struct FTileAsset : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Structure")
	UStaticMesh* StaticMesh = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Structure")
	USkeletalMesh* SkeletalMesh= nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Structure")
	UAnimSequence* Animation = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Structure")
	EGameplayTagMode TagMode = EGameplayTagMode::MatchAny;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Structure")
	FGameplayTagContainer SpawnConditionTags;
};
