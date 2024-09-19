#include "Woodcutter.h"

#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

void AWoodcutter::ValidateStatus()
{
	if (GetStatus() == ECivilianStatus::Idle)
	{
		if (CurrentTile->EcoValues->GetTrees() > 0)
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
		if (CurrentTile->EcoValues->GetTrees() <= 0)
		{
			if (TryFindPath())
				SetStatus(ECivilianStatus::Moving);
			else
				SetStatus(ECivilianStatus::Idle);
		}
	}
}

void AWoodcutter::Work()
{
	CurrentTile->EcoValues->SubtractTrees(1);
	Settlement->Resources.Wood += WorkAmount;
}

bool AWoodcutter::TryFindPath()
{
	Path = ATileMap::FindPathToNearestTile(CurrentTile, EEntityType::Civilian, [this](const ATile* Tile)
	{
		return Tile->EcoValues->GetTrees() > 0 && Settlement->IsBorderingUnclaimedTile(Tile);
	});
	if(!Path.IsEmpty()) return true;
	Path = ATileMap::FindPathToNearestTile(CurrentTile, EEntityType::Civilian, [](const ATile* Tile)
	{
		return Tile->EcoValues->GetTrees() > 0;
	});
	return !Path.IsEmpty();
}
