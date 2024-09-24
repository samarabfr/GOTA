#include "Hunter.h"

#include "CivilianDataAsset.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

AHunter::AHunter()
{
	Mesh->SetStaticMesh(CivilianDataAsset->HunterMesh);
}

void AHunter::ValidateStatus()
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

void AHunter::Work()
{
	CurrentTile->EcoValues->SubtractWildlife(1);
	FGameResources WorkResources = FGameResources();
	WorkResources.Food = WorkAmount;
	Settlement->AddResources(WorkResources);
}

bool AHunter::TryFindPath()
{
	bool HasBorderingTileWithTrees = false;
	for (ATile* Tile : Settlement->BorderingUnclaimedTiles)
	{
		if (IsTileValidForWork(Tile))
		{
			HasBorderingTileWithTrees = true;
			break;
		}
	}
	if (HasBorderingTileWithTrees)
	{
		Path = GameState->TileMap->FindPathToNearestTile(CurrentTile, EEntityType::Civilian, [this](const ATile* Tile)
		{
			return IsTileValidForWork(Tile) && Settlement->IsBorderingUnclaimedTile(Tile);
		});
	}
	else
	{
		Path = GameState->TileMap->FindPathToNearestTile(CurrentTile, EEntityType::Civilian, [this](const ATile* Tile)
		{
			return IsTileValidForWork(Tile);
		});
	}
	return !Path.IsEmpty();
}

bool AHunter::IsTileValidForWork(const ATile* Tile) const
{
	return Tile->EcoValues->GetWildlife() > 0;
}
