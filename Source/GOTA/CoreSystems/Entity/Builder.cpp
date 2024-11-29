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
	if (Settings)
		MeshComponent->SetStaticMesh(Settings->BuilderMesh);
}

void ABuilder::ValidateStatus()
{
	if (GetStatus() == ECivilianStatus::Idle)
	{
		if (IsTileValidForWork(CurrentTile))
			SetStatus(ECivilianStatus::Working);
		else if (TryFindPath())
			SetStatus(ECivilianStatus::Moving);
	}
	else if (GetStatus() == ECivilianStatus::Moving)
	{
		if ((Path.IsEmpty() || !Path[Path.Num() - 1]->AcceptsCivilian()) && !TryFindPath())
			SetStatus(ECivilianStatus::Idle);
	}
	else if (GetStatus() == ECivilianStatus::Working)
	{
		if (!IsTileValidForWork(CurrentTile))
		{
			if (TryFindPath())
				SetStatus(ECivilianStatus::Moving);
			else
				SetStatus(ECivilianStatus::Idle);
		}
	}
}

void ABuilder::Work()
{
	int32 WorkAmountLeft = WorkAmount;
	const FGameResources ResourcesProgress = CurrentTile->GetBuilding()->GetConstructionProgress();
	FGameResources ResourcesProgressToAdd = FGameResources();
	// Food
	const int32 FoodNeeded = CurrentTile->GetBuilding()->Settings->Cost.Food - ResourcesProgress.Food;
	if (FoodNeeded > 0 && WorkAmountLeft > 0)
	{
		const int32 DoneWork = FMath::Min(FMath::Min(WorkAmountLeft, FoodNeeded),
		                                  Building->Settlement->GetResources().Food);
		ResourcesProgressToAdd.Food = DoneWork;
		WorkAmountLeft -= DoneWork;
	}
	// Wood
	const int32 WoodNeeded = CurrentTile->GetBuilding()->Settings->Cost.Wood - ResourcesProgress.Wood;
	if (WoodNeeded > 0 && WorkAmountLeft > 0)
	{
		const int32 DoneWork = FMath::Min(FMath::Min(WorkAmountLeft, WoodNeeded),
		                                  Building->Settlement->GetResources().Wood);
		ResourcesProgressToAdd.Wood = DoneWork;
		WorkAmountLeft -= DoneWork;
	}
	// Stone
	const int32 StoneNeeded = CurrentTile->GetBuilding()->Settings->Cost.Stone - ResourcesProgress.Stone;
	if (StoneNeeded > 0 && WorkAmountLeft > 0)
	{
		const int32 DoneWork = FMath::Min(FMath::Min(WorkAmountLeft, StoneNeeded),
		                                  Building->Settlement->GetResources().Stone);
		ResourcesProgressToAdd.Stone = DoneWork;
		WorkAmountLeft -= DoneWork;
	}
	CurrentTile->GetBuilding()->SetConstructionProgress(ResourcesProgress + ResourcesProgressToAdd);
	Building->Settlement->S_RemoveResources(ResourcesProgressToAdd);
}

bool ABuilder::TryFindPath()
{
	bool HasValidTiles = false;
	for (ATile* Tile : Building->Settlement->ClaimedTiles)
	{
		if (IsTileValidForWork(Tile))
		{
			HasValidTiles = true;
			break;
		}
	}
	if (!HasValidTiles) return false;
	Path = GameState->GetTileMap()->FindPathToNearestTile(CurrentTile, EEntityType::Civilian, [this](const ATile* Tile)
	{
		return IsTileValidForWork(Tile);
	});
	return !Path.IsEmpty();
}

bool ABuilder::IsTileValidForWork(const ATile* Tile) const
{
	return Tile->GetBuilding() && Tile->GetBuilding()->GetIsUnderConstruction();
}
