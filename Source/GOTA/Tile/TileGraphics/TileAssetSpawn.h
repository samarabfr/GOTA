#pragma once

#include "SpawnPoint.h"
#include "TileAsset.h"

// Unspawned = is not spawned
// Unfinished = is spawned, but not finished constructing/growing
// Finished = is spawned and completed
// Destroyed = is spawned and destroyed
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

	void ApplyAssetRotation();
};
