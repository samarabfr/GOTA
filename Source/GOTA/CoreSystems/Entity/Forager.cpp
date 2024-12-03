#include "Forager.h"

#include "CivilianSettings.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

AForager::AForager()
{
	if (GetSettings())
		GetMeshComponent()->SetStaticMesh(GetSettings()->ForagerMesh);
}

void AForager::S_Work()
{
	GetCurrentTile()->EcoValues->SubtractForage(1);
	FGameResources WorkResources = FGameResources();
	WorkResources.Food = GetWorkAmount();
	GetBuilding()->GetSettlement()->S_AddResources(WorkResources);
}

bool AForager::IsTileValidForWork(ATile* Tile) const
{
	return Tile->EcoValues->GetForage() > 0;
}
