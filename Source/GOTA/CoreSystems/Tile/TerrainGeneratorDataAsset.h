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
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FNoiseParameter NoiseParameter;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float DistanceToMiddlePointMaxHeightFactor = 4000;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float DistanceToMiddlePointGradientFactor = 0.2;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float HeightOffset = -1000;
};
