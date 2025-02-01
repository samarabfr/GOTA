#include "Migrant.h"

#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/Population.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

void AMigrant::S_Work()
{
	GetCurrentTile()->GetBuilding()->GetPopulation()->S_IncreaseSize(1);
	--Size;
}

bool AMigrant::IsTileValidForWork(const ATile* Tile) const
{
	return Tile->GetBuilding()
		&& Tile->GetBuilding()->GetPopulation()->GetSize() < Tile->GetBuilding()->GetPopulation()->GetMaxSize()
		&& Tile->GetClaimant()
		&& Tile->GetClaimant() == GetOriginBuilding()->GetSettlement();
}

void AMigrant::SetSize(const int32 NewSize)
{
	Size = NewSize;
}
