// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/Utility/Enums.h"
#include "GOTA/Tile/HexCoords.h"

struct FGeneratedTileInfo
{
	// general stuff
	FHexCoords HexCoords = FHexCoords(0,0);

	FGeneratedTileInfo* Neighbors[6];

	// shape generation
	bool IsLand = true;

	bool IsLandConnectedToMainIsland = false;

	bool IsWaterConnectedToOcean = false;

	// Height generation
	float Height = 1;
	
	int8 OceanDistance = -1;
	
	int8 VolcanoDistance = -1;
	
	// River
	bool HasRiver = false;

	bool HasRiverSpring = false;

	bool HasRiverEnd = false;

	int8 TriesAsStartPosition = 0;

	int32 RiverDistance = -1;
	
	//Biome
	EBiome Biome = EBiome::Gras;

	// starting Positions
	bool IsColonistStart = false;

	bool IsNativeStart = false;
	
	int32 ColonistsDistance = MAX_int32;

	TArray<bool> RiverConnections;

	bool CleanedUpRiverConnections = false;
};
