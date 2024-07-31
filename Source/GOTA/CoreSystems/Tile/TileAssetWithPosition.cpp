#include "TileAssetWithPosition.h"

FTileAssetWithPosition::FTileAssetWithPosition(FName TileAssetRowName_, const FSpawnPoint& SpawnPoint_)
{
	TileAssetRowName = TileAssetRowName_;
	SpawnPoint = SpawnPoint_;
}

FTileAssetWithPosition::FTileAssetWithPosition()
{
}
