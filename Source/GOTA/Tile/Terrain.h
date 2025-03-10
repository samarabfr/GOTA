// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GOTA/Utility/Enums.h"
#include "Terrain.generated.h"

USTRUCT()
struct FTerrain
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly)
	bool bIsRiver = false;
	
	UPROPERTY(VisibleInstanceOnly)
	EBiome Biome = EBiome::Gras;
	
	UPROPERTY(VisibleInstanceOnly)
	int32 OceanDistance = -1;

	UPROPERTY(VisibleInstanceOnly)
	float NormalizedOceanDistance = -1;

	UPROPERTY(VisibleInstanceOnly)
	int32 RiverDistance = -1;

	UPROPERTY(VisibleInstanceOnly)
	float NormalizedRiverDistance = -1;

	UPROPERTY(VisibleInstanceOnly)
	int32 VolcanoDistance = -1;

	UPROPERTY(VisibleInstanceOnly)
	float NormalizedVolcanoDistance = -1;

	UPROPERTY(VisibleInstanceOnly)
	TArray<bool> RiverConnections;
};
