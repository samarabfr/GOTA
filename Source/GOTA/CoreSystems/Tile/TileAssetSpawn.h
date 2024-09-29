#pragma once

#include "SpawnPoint.h"
#include "TileAsset.h"

enum class ESpawnState
{
	Unspawned,
	Unfinished,
	Finished,
	Destroyed
};

struct FTileAssetSpawn
{
	FSpawnPoint SpawnPoint;
	
	bool bIsSpawned = false;
	
	ESpawnState SpawnState = ESpawnState::Unspawned;

	UTileAsset* TileAsset = nullptr;

	FPrimitiveInstanceId InstanceId;
	
	UStaticMesh* GetMeshForSpawnState(const ESpawnState ForSpawnState) const;
};
