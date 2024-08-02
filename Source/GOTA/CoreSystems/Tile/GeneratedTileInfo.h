// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexCoords.h"
#include "GOTA/CoreSystems/Utility/Enums.h"

struct FGeneratedTileInfo
{
	FHexCoords HexCoords = FHexCoords(0,0);

	FGeneratedTileInfo* Neighbors[6];

	int32 NeighborCount = 0;

	// shape generation
	bool ShouldGenerate = true;

	bool ConnectedToMainIsland = false;

	bool ConnectedToOcean = false;

	// Height generation
	float Height = 0;
	
	// Details
	bool HasRiver = false;

	EBiome Biome = EBiome::Gras;
};
