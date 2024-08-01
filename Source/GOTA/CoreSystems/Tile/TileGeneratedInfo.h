// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexCoords.h"
#include "GOTA/CoreSystems/Utility/Enums.h"

struct FTileGeneratedInfo
{
	FHexCoords HexCoords = FHexCoords(0,0);

	float Height = 0;

	bool HasRiver = false;

	EBiome Biome = EBiome::Gras;

	bool ShouldGenerate = true;

	bool ConnectedToMainIsland = false;

	bool ConnectedToOcean = false;

	FTileGeneratedInfo* Neighbors[6];

	int32 NeighborCount = 0;
};
