#include "Woodcutter.h"

#include "GOTA/CoreSystems/Faction/Building/ResourceStorage.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

void AWoodcutter::S_Work()
{
	if (IsInventoryFull() || !GetCurrentTile()) return;
	GetCurrentTile()->EcoValues->SubtractTrees(1);
	GetStorage()->S_Add(GetWorkAmount());
}

bool AWoodcutter::IsTileValidForWork(const ATile* Tile) const
{
	if (!Tile) return false;
	return Tile->EcoValues->GetTrees() > 0;
}

bool AWoodcutter::S_TryFindPathToBestWorkTile()
{
	return S_TryFindPathToWorkTileClosestToSettlement();
}
