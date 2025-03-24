#include "Forager.h"

#include "GOTA/Tile/EcoValues.h"
#include "GOTA/Utility/ResourceStorage.h"
#include "GOTA/Tile/Tile.h"

void AForager::S_Work()
{
	if (IsInventoryFull()) return;
	GetCurrentTile()->EcoValues->SubtractForage(1);
	GetStorage()->S_Add(GetWorkAmount());
}

bool AForager::IsTileValidForWork(const ATile* Tile) const
{
	if (!Tile)
		return false;
	return Tile->EcoValues->GetForage() > 0;
}

TArray<ATile*> AForager::FindPathToBestWorkTile() const
{
	return FindPathToWorkTileClosestToSettlement();
}
