#include "Woodcutter.h"

#include "CivilianSettings.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

AWoodcutter::AWoodcutter()
{
	if (GetSettings())
		GetMeshComponent()->SetStaticMesh(GetSettings()->WoodCutterMesh);
}

void AWoodcutter::S_Work()
{
	GetCurrentTile()->EcoValues->SubtractTrees(1);
	FGameResources WorkResources = FGameResources();
	WorkResources.Wood = GetWorkAmount();
	GetBuilding()->GetSettlement()->S_AddResources(WorkResources);
}

bool AWoodcutter::IsTileValidForWork(const ATile* Tile) const
{
	return Tile->EcoValues->GetTrees() > 0;
}


