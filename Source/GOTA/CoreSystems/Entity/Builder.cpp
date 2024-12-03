#include "Builder.h"

#include "CivilianSettings.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

ABuilder::ABuilder()
{
	if (GetSettings())
		GetMeshComponent()->SetStaticMesh(GetSettings()->BuilderMesh);
}

void ABuilder::S_Work()
{
	int32 WorkAmountLeft = GetWorkAmount();
	const FGameResources ResourcesProgress = GetCurrentTile()->GetBuilding()->GetConstructionProgress();
	FGameResources ResourcesProgressToAdd = FGameResources();
	// Food
	const int32 FoodNeeded = GetCurrentTile()->GetBuilding()->GetSettings()->Cost.Food - ResourcesProgress.Food;
	if (FoodNeeded > 0 && WorkAmountLeft > 0)
	{
		const int32 DoneWork = FMath::Min(FMath::Min(WorkAmountLeft, FoodNeeded),
		                                  GetBuilding()->GetSettlement()->GetResources().Food);
		ResourcesProgressToAdd.Food = DoneWork;
		WorkAmountLeft -= DoneWork;
	}
	// Wood
	const int32 WoodNeeded = GetCurrentTile()->GetBuilding()->GetSettings()->Cost.Wood - ResourcesProgress.Wood;
	if (WoodNeeded > 0 && WorkAmountLeft > 0)
	{
		const int32 DoneWork = FMath::Min(FMath::Min(WorkAmountLeft, WoodNeeded),
		                                  GetBuilding()->GetSettlement()->GetResources().Wood);
		ResourcesProgressToAdd.Wood = DoneWork;
		WorkAmountLeft -= DoneWork;
	}
	// Stone
	const int32 StoneNeeded = GetCurrentTile()->GetBuilding()->GetSettings()->Cost.Stone - ResourcesProgress.Stone;
	if (StoneNeeded > 0 && WorkAmountLeft > 0)
	{
		const int32 DoneWork = FMath::Min(FMath::Min(WorkAmountLeft, StoneNeeded),
		                                  GetBuilding()->GetSettlement()->GetResources().Stone);
		ResourcesProgressToAdd.Stone = DoneWork;
		WorkAmountLeft -= DoneWork;
	}
	GetCurrentTile()->GetBuilding()->SetConstructionProgress(ResourcesProgress + ResourcesProgressToAdd);
	GetBuilding()->GetSettlement()->S_RemoveResources(ResourcesProgressToAdd);
}

bool ABuilder::IsTileValidForWork(const ATile* Tile) const
{
	if(!Tile->GetBuilding())
		return false;
	if(Tile->GetClaimant() != GetBuilding()->GetSettlement())
		return false;
	if(!Tile->GetBuilding()->GetIsUnderConstruction())
		return false;
	return true;
}
