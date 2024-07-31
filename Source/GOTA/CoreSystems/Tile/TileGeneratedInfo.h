// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexCoords.h"
#include "GOTA/CoreSystems/Utility/Enums.h"

struct FTileGeneratedInfo
{
	FHexCoords HexCoords;

	float Height;

	bool HasRiver;

	EBiome Biome;
};
