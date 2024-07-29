#include "TileAssetSpawn.h"

void FTileAssetSpawn::RefreshPosition()
{
	if(!StaticMeshComponent || !SpawnPoint) return;
	StaticMeshComponent->SetRelativeLocation(SpawnPoint->LocationOnTile);
	StaticMeshComponent->SetRelativeRotation(FRotator(0.0f, SpawnPoint->DefaultRotation, 0.0f));
}
