// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnPoint.h"
#include "TileAsset.h"

struct FTileAssetSpawn
{
	FSpawnPoint* SpawnPoint = nullptr;
	
	bool bIsSpawned = false;

	FTileAsset* TileAsset = nullptr;

	UStaticMeshComponent* StaticMeshComponent = nullptr;

	void RefreshPosition();
};
