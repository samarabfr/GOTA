// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnPoint.h"
#include "TileAssetDA.h"

struct FTileAssetSpawn
{
	FSpawnPoint SpawnPoint;
	
	bool bIsSpawned = false;

	UTileAssetDA* TileAsset = nullptr;

	FPrimitiveInstanceId InstanceId;
};
