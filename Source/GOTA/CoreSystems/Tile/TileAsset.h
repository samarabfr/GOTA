// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagRule.h"
#include "GameFramework/Actor.h"
#include "TileAsset.generated.h"

UENUM(BlueprintType)
enum class ERotationMode : uint8
{
	SpawnPointRotation UMETA(DisplayName = "Default Spawn Point Rotation"),
	Random90Degree UMETA(DisplayName = "90 Degree Random"),
	Random360Degree UMETA(DisplayName = "360 Degree Random"),
};

USTRUCT(BlueprintType)
struct FTileAsset : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Graphics")
	UStaticMesh* StaticMesh = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Graphics")
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Graphics")
	UAnimSequence* Animation = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Graphics")
	ERotationMode RotationMode = ERotationMode::SpawnPointRotation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawn Bias")
	int32 SpawnBias = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawn Bias")
	bool bUseDistanceToOceanBiasMultiplier = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawn Bias")
	UCurveFloat* DistanceToOceanBiasMultiplier = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawn Bias")
	bool bUseDistanceToRiverBiasMultiplier = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawn Bias")
	UCurveFloat* DistanceToRiverBiasMultiplier = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawn Condition")
	TArray<FGameplayTagRule> GameplayTagRules;

	bool IsValidFor(const FGameplayTagContainer& GameplayTagContainer);
};
