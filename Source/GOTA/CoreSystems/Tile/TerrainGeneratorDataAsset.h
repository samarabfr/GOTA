// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NoiseParameter.h"
#include "Engine/DataAsset.h"
#include "TerrainGeneratorDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API UTerrainGeneratorDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="ShapeGen")
	float LandToArraySizeRatio = 0.5;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="ShapeGen")
	float ShapeDistanceToMiddlePointFactor = 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="ShapeGen")
	FRuntimeFloatCurve DistanceToMiddlePointCurve;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="ShapeGen")
	FNoiseParameter ShapeNoiseParameter;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="ShapeGen")
	float ShapeHeightOffset = -1000;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="ShapeGen")
	float IsLandThreshold;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="ShapeGen")
	float VolcanoSpawnOceanDistancePercentageThreshold = 0.8;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="HeightGen")
	int32 MaxHeightStepDifference = 5;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="HeightGen")
	float HeightStep = 100;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="HeightGen")
	float HeightOffset = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="HeightGen")
	FRuntimeFloatCurve OceanDistanceCurve;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="HeightGen")
	float OceanDistanceFactor = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="HeightGen")
	FRuntimeFloatCurve VolcanoDistanceCurve;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="HeightGen")
	float VolcanoDistanceFactor = 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="HeightGen")
	float VolcanoOffsetToLowestNeighbor = -500;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="HeightGen")
	FNoiseParameter HeightNoiseParameter;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="BiomeGen")
	int32 BeachSize = 5;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="BiomeGen")
	float MinTotalBeachPercentage = 0.5;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="BiomeGen")
	float MinTotalMountainPercentage = 0.1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="RiverGen")
	int32 MaximumRiverConnections = 3;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="RiverGen")
	float MinTotalRiverPercentage = 0.3;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="RiverGen")
	int32 MinRiverLength = 5;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="RiverGen")
	int32 MinBranchLength = 2;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="RiverGen")
	float OceanStartingChance = 0.5;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="RiverGen")
	int32 MaxTriesForRiverStartPositions = 10;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="SettlementGen")
	int32 IterationsForceCalc = 1000;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="SettlementGen")
	float KConstantForceCalc = 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="SettlementGen")
	float DeltaTimeForceCalc = 0.01;
};
