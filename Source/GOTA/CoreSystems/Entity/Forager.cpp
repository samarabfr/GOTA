#include "Forager.h"

#include "CivilianSettings.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

AForager::AForager()
{
	if (Settings)
		MeshComponent->SetStaticMesh(Settings->ForagerMesh);
}

void AForager::ValidateStatus()
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

void AForager::Work()
{
	CurrentTile->EcoValues->SubtractForage(1);
	FGameResources WorkResources = FGameResources();
	WorkResources.Food = WorkAmount;
	Building->Settlement->AddResources(WorkResources);
}

bool AForager::TryFindPath()
{
	bool HasBorderingTileWithTrees = false;
	for (ATile* Tile : Building->Settlement->BorderingUnclaimedTiles)
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
			return IsTileValidForWork(Tile) && Building->Settlement->IsBorderingUnclaimedTile(Tile);
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

bool AForager::IsTileValidForWork(const ATile* Tile) const
{
	return Tile->EcoValues->GetForage() > 0;
}
