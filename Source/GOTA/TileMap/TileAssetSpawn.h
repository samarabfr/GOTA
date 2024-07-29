// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnPoint.h"
#include "TileAsset.h"

struct FTileAssetSpawn
{
	FSpawnPoint SpawnPoint;
	
	bool bIsSpawned = false;

	FTileAsset* TileAsset = nullptr;

	FPrimitiveInstanceId InstanceId;
};
