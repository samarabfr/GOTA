#include "Builder.h"

#include "CivilianSettings.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"

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
	if (!Tile->GetBuilding())
		return false;
	if (Tile->GetClaimant() != GetBuilding()->GetSettlement())
		return false;
	if (!Tile->GetBuilding()->GetIsUnderConstruction())
		return false;
	return true;
}

TArray<ATile*> ABuilder::FindBestWorkTiles()
{
	if (!GetBuilding() ||
		!GetBuilding()->GetSettlement() ||
		GetBuilding()->GetSettlement()->ClaimedTiles.Num() <= 0)
		return TArray<ATile*>();
	// get work tiles with builders and the lowest count of builders
	TMap<ATile*, int32> WorkTilesWithBuilders;
	int32 LowestCount = INT32_MAX;
	for (ATile* ClaimedTile : GetBuilding()->GetSettlement()->ClaimedTiles)
	{
		if (!IsTileValidForWork(ClaimedTile) || !ClaimedTile->AcceptsCivilian()) continue;
		int32 Count = 0;
		for (ACivilian* Civilian : ClaimedTile->GetCivilians())
		{
			if (Civilian &&
				Civilian != this &&
				Civilian->IsA(GetBuilding()->GetSettings()->CivilianClass))
				++Count;
		}
		if (Count <= LowestCount)
		{
			LowestCount = Count;
			WorkTilesWithBuilders.Add(ClaimedTile, Count);
		}
	}
	if (WorkTilesWithBuilders.IsEmpty()) return TArray<ATile*>();
	// get only tiles with the lowest count
	TArray<ATile*> BestTiles;
	for (auto WorkTilesWithBuilder : WorkTilesWithBuilders)
	{
		if (WorkTilesWithBuilder.Value == LowestCount) BestTiles.Add(WorkTilesWithBuilder.Key);
	}
	return BestTiles;
}

bool ABuilder::TryFindPathToBestWorkTile()
{
	TArray<ATile*> BestTiles = FindBestWorkTiles();
	if(BestTiles.IsEmpty()) return false;
	// get path the closest
	const TArray<ATile*> NewPath = GetGameState()->GetTileMap()->FindPathToNearestTile(
		GetCurrentTile(), EEntityType::Civilian,
		[this, BestTiles](const ATile* Tile)
		{
			return BestTiles.Contains(Tile);
		});
	SetPath(NewPath);
	return !IsPathEmpty();
}

bool ABuilder::IsCurrentTileAmongBestWorkTiles()
{
	return FindBestWorkTiles().Contains(GetCurrentTile());
}
