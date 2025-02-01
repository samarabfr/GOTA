#include "Woodcutter.h"

#include "CivilianSettings.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

void AWoodcutter::S_Work()
{
	if (IsInventoryFull()) return;
	GetCurrentTile()->EcoValues->SubtractTrees(1);
	FGameResources WorkResources = FGameResources();
	WorkResources.Wood = GetWorkAmount();
	S_AddResources(WorkResources);
}

bool AWoodcutter::IsTileValidForWork(const ATile* Tile) const
{
	return Tile->EcoValues->GetTrees() > 0;
}

bool AWoodcutter::S_TryFindPathToBestWorkTile()
{
	return S_TryFindPathToWorkTileClosestToSettlement();
}
