#include "Builder.h"

#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

void ABuilder::S_Work()
{
	if (!GetCurrentTile() || !GetCurrentTile()->GetBuilding())
		return;
	int32 WorkAmountLeft = GetWorkAmount();
	const FConstructionResources ResourcesProgress = GetCurrentTile()->GetBuilding()->GetConstructionProgress();
	FConstructionResources ResourcesProgressToAdd = FConstructionResources();
	// Food
	const int32 FoodNeeded = GetCurrentTile()->GetBuilding()->GetSettings()->Cost.Food - ResourcesProgress.Food;
	if (FoodNeeded > 0 && WorkAmountLeft > 0)
	{
		const int32 DoneWork = FMath::Min(FMath::Min(WorkAmountLeft, FoodNeeded),
		                                  GetOriginBuilding()->GetSettlement()->GetResources().Food);
		ResourcesProgressToAdd.Food = DoneWork;
		WorkAmountLeft -= DoneWork;
	}
	// Wood
	const int32 WoodNeeded = GetCurrentTile()->GetBuilding()->GetSettings()->Cost.Wood - ResourcesProgress.Wood;
	if (WoodNeeded > 0 && WorkAmountLeft > 0)
	{
		const int32 DoneWork = FMath::Min(FMath::Min(WorkAmountLeft, WoodNeeded),
		                                  GetOriginBuilding()->GetSettlement()->GetResources().Wood);
		ResourcesProgressToAdd.Wood = DoneWork;
		WorkAmountLeft -= DoneWork;
	}
	// Stone
	const int32 StoneNeeded = GetCurrentTile()->GetBuilding()->GetSettings()->Cost.Stone - ResourcesProgress.Stone;
	if (StoneNeeded > 0 && WorkAmountLeft > 0)
	{
		const int32 DoneWork = FMath::Min(FMath::Min(WorkAmountLeft, StoneNeeded),
		                                  GetOriginBuilding()->GetSettlement()->GetResources().Stone);
		ResourcesProgressToAdd.Stone = DoneWork;
		WorkAmountLeft -= DoneWork;
	}
	GetCurrentTile()->GetBuilding()->S_SetConstructionProgress(ResourcesProgress + ResourcesProgressToAdd);
	GetOriginBuilding()->GetSettlement()->S_RemoveResources(ResourcesProgressToAdd);
}

bool ABuilder::IsTileValidForWork(const ATile* Tile) const
{
	if (!Tile)
	return false;
	if (!Tile->GetBuilding())
		return false;
	if (Tile->GetClaimant() != GetOriginBuilding()->GetSettlement())
		return false;
	if (!Tile->GetBuilding()->GetIsUnderConstruction())
		return false;
	return true;
}
