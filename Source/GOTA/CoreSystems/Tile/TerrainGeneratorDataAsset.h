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
	float ShouldGenerateThreshhold;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="HeightGen")
	FNoiseParameter HeightNoiseParameter;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="HeightGen")
	float DistanceToMiddlePointMaxHeightFactor = 4000;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="HeightGen")
	float DistanceToMiddlePointGradientFactor = 0.2;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="HeightGen")
	float HeightOffset = -1000;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="BiomeGen")
	float VolcanoSpawnOceanDistancePercentageThreshold = 0.8;
};
