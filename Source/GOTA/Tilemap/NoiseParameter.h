// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FastNoiseWrapper.h"
#include "GameFramework/Actor.h"
#include "NoiseParameter.generated.h"

USTRUCT(BlueprintType)
struct FNoiseParameter
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float NoiseFactor = 500;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EFastNoise_NoiseType NoiseType = EFastNoise_NoiseType::Simplex;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Frequency = 0.1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EFastNoise_Interp Interpolation = EFastNoise_Interp::Quintic;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EFastNoise_FractalType FractalType = EFastNoise_FractalType::FBM;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 Octaves = 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Lacunarity = 2;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float gain = 0.5;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float CellularJitter = 0.45;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EFastNoise_CellularDistanceFunction CellularDistanceFunction = EFastNoise_CellularDistanceFunction::Euclidean;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EFastNoise_CellularReturnType CellularReturnType = EFastNoise_CellularReturnType::CellValue;
};
