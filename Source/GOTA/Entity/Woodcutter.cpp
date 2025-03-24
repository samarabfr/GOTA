#include "Woodcutter.h"

#include "GOTA/Tile/EcoValues.h"
#include "GOTA/Utility/ResourceStorage.h"
#include "GOTA/Tile/Tile.h"

void AWoodcutter::S_Work()
{
	if (IsInventoryFull()) return;
	GetCurrentTile()->EcoValues->SubtractTrees(1);
	GetStorage()->S_Add(GetWorkAmount());
}

bool AWoodcutter::IsTileValidForWork(const ATile* Tile) const
{
	if (!Tile)
		return false;
	return Tile->EcoValues->GetTrees() > 0;
}

TArray<ATile*> AWoodcutter::FindPathToBestWorkTile() const
{
	return FindPathToWorkTileClosestToSettlement();
}
