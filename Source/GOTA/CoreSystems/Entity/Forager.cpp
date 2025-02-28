#include "Forager.h"

#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

void AForager::S_Work()
{
	if (IsInventoryFull()) return;
	GetCurrentTile()->EcoValues->SubtractForage(1);
	FConstructionResources WorkResources = FConstructionResources();
	WorkResources.Food = GetWorkAmount();
	S_AddResources(WorkResources);
}

bool AForager::IsTileValidForWork(const ATile* Tile) const
{
	return Tile->EcoValues->GetForage() > 0;
}

bool AForager::S_TryFindPathToBestWorkTile()
{
	return S_TryFindPathToWorkTileClosestToSettlement();
}
