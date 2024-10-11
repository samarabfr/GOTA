#include "Migrant.h"

#include "CivilianSettings.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

AMigrant::AMigrant()
{
	if (Settings)
		MeshComponent->SetStaticMesh(Settings->MigrantMesh);
}

void AMigrant::ValidateStatus()
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
	if (Size <= 0)
	{
		Destroy();
	}
}

void AMigrant::Work()
{
	CurrentTile->GetBuilding()->Population->IncreaseSize(1);
	--Size;
}

bool AMigrant::TryFindPath()
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
	Path = GameState->TileMap->FindPathToNearestTile(CurrentTile, EEntityType::Civilian, [this](const ATile* Tile)
	{
		return IsTileValidForWork(Tile);
	});
	return !Path.IsEmpty();
}

bool AMigrant::IsTileValidForWork(const ATile* Tile) const
{
	return Tile->GetBuilding()
		&& Tile->GetBuilding()->Population->GetSize() < Tile->GetBuilding()->Population->GetMaxSize()
		&& Tile->GetClaimant()
		&& Tile->GetClaimant() == Building->Settlement;
}

void AMigrant::SetSize(const int32 NewSize)
{
	Size = NewSize;
}
