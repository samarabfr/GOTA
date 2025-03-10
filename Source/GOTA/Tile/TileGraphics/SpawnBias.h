// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnBias.generated.h"

struct FTerrain;

USTRUCT(BlueprintType)
struct FSpawnBias : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawn Bias")
	int32 Base = 100;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawn Bias")
	bool bUseDistanceToOceanBiasMultiplier = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawn Bias")
	UCurveFloat* DistanceToOceanBiasMultiplier = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawn Bias")
	bool bUseDistanceToRiverBiasMultiplier = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawn Bias")
	UCurveFloat* DistanceToRiverBiasMultiplier = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawn Bias")
	bool bUseDistanceToVolcanoBiasMultiplier = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawn Bias")
	UCurveFloat* DistanceToVolcanoBiasMultiplier = nullptr;

	int32 GetBiasAfterMultipliers(const FTerrain& Terrain) const;
};
