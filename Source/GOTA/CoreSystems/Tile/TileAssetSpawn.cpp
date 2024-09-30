#include "TileAssetSpawn.h"

UStaticMesh* FTileAssetSpawn::GetMeshForSpawnState(const ESpawnState ForSpawnState) const
{
	if (!TileAsset) return nullptr;
	switch (ForSpawnState)
	{
	case ESpawnState::Unfinished:
		return TileAsset->MeshUnfinished;
	case ESpawnState::Finished:
		return TileAsset->MeshFinished;
	case ESpawnState::Destroyed:
		return TileAsset->MeshDestroyed;
	default:
		return nullptr;
	}
}
