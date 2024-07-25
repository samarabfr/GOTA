// Fill out your copyright notice in the Description page of Project Settings.

#include "TileMap.h"
#include "HexCoords.h"
#include "GOTA/Faction/Settlement.h"
#include "Net/UnrealNetwork.h"

void ATileMap::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATileMap, Tiles);
	DOREPLIFETIME(ATileMap, MapRadius);
	DOREPLIFETIME(ATileMap, MapSize);
}

ATileMap::ATileMap()
{
	bReplicates = true;
	bAlwaysRelevant = true;
}

int32 ATileMap::MapOffset = 0;

void ATileMap::Init(int32 Init_MapRadius)
{
	MapRadius = FMath::Max(Init_MapRadius, 0);
	MapOffset = MapRadius;
	MapSize = MapRadius * 2 + 1;
	for (int32 i = 0; i < MapSize * MapSize; ++i)
	{
		Tiles.Add(nullptr);
	}
	InitializeArray();
}

void ATileMap::OnRep_MapRadius() const
{
	MapOffset = MapRadius;
}

void ATileMap::InitializeArray()
{
	TilesArray = new ATile**[MapSize];
	for (int32 i = 0; i < MapSize; ++i)
	{
		TilesArray[i] = new ATile*[MapSize];
		for (int32 j = 0; j < MapSize; ++j)
		{
			TilesArray[i][j] = nullptr; // Initialize to nullptr or any default value
		}
	}
}


bool ATileMap::TryAddTile(FHexCoords HexCoords, ATile* Tile)
{
	// Check for out of bounds
	if (HexCoords.Q >= MapSize || HexCoords.R >= MapSize ||
		HexCoords.Q < 0 || HexCoords.R < 0)
	{
		return false;
	}
	if (DoesTileExist(HexCoords) || !Tile)
	{
		return false;
	}
	Tiles[HexCoords.Q * MapSize + HexCoords.R] = Tile;
	TilesArray[HexCoords.Q][HexCoords.R] = Tile;
	Tile->HexCoords = HexCoords;
	// set neigbors on new tile
	Tile->Neighbors[0] = GetTileFast(FHexCoords(HexCoords.Q, HexCoords.R - 1));
	Tile->Neighbors[1] = GetTileFast(FHexCoords(HexCoords.Q + 1, HexCoords.R - 1));
	Tile->Neighbors[2] = GetTileFast(FHexCoords(HexCoords.Q + 1, HexCoords.R));
	Tile->Neighbors[3] = GetTileFast(FHexCoords(HexCoords.Q, HexCoords.R + 1));
	Tile->Neighbors[4] = GetTileFast(FHexCoords(HexCoords.Q - 1, HexCoords.R + 1));
	Tile->Neighbors[5] = GetTileFast(FHexCoords(HexCoords.Q - 1, HexCoords.R));
	// set new tile on neighbors
	if (Tile->Neighbors[0]) Tile->Neighbors[0]->Neighbors[3] = Tile;
	if (Tile->Neighbors[1]) Tile->Neighbors[1]->Neighbors[4] = Tile;
	if (Tile->Neighbors[2]) Tile->Neighbors[2]->Neighbors[5] = Tile;
	if (Tile->Neighbors[3]) Tile->Neighbors[3]->Neighbors[0] = Tile;
	if (Tile->Neighbors[4]) Tile->Neighbors[4]->Neighbors[1] = Tile;
	if (Tile->Neighbors[5]) Tile->Neighbors[5]->Neighbors[2] = Tile;
	// init
	Tile->Init();
	return true;
}

ATile* ATileMap::GetTile(FHexCoords HexCoords)
{
	// Check for out of bounds
	if (HexCoords.Q >= MapSize || HexCoords.R >= MapSize ||
		HexCoords.Q < 0 || HexCoords.R < 0)
	{
		return nullptr;
	}
	// Check in 2D-Array
	return Tiles[HexCoords.Q * MapSize + HexCoords.R];
}

ATile* ATileMap::GetTileFast(FHexCoords HexCoords)
{
	// Check for out of bounds
	if (HexCoords.Q >= MapSize || HexCoords.R >= MapSize ||
		HexCoords.Q < 0 || HexCoords.R < 0)
	{
		return nullptr;
	}
	return TilesArray[HexCoords.Q][HexCoords.R];
}

bool ATileMap::DoesTileExist(FHexCoords HexCoords)
{
	if (GetTile(HexCoords))
	{
		return true;
	}
	return false;
}

ATile* ATileMap::GetRandomTile()
{
	// Try to get a Tile randomly
	const int32 MaxTries = 100;
	for (int32 i = 0; i < MaxTries; ++i)
	{
		const int32 RandomIndex = FMath::RandRange(0, MapSize * MapSize - 1);
		if (ATile* RandomTile = Tiles[RandomIndex])
		{
			return RandomTile;
		}
	}
	// Get the first Tile you can find in the TileMap 
	for (ATile* Tile : Tiles)
	{
		if (Tile)
		{
			return Tile;
		}
	}
	return nullptr;
}

void ATileMap::CalculateTurn()
{
	ATile::bFreezeGrowthChanges = false;
	for (ATile* Tile : Tiles)
	{
		if (Tile) Tile->CalculateTurn();
	}
	ATile::bFreezeGrowthChanges = true;
	for (ATile* Tile : Tiles)
	{
		if (Tile)
		{
			Tile->CalculateTreeGrowthChange();
			Tile->CalculateWildlifeGrowthChange();
		}
	}
}

TArray<ATile*> ATileMap::GetPath(ATile* Start, ATile* End, EAffiliation Affiliation)
{
	//https://www.redblobgames.com/pathfinding/a-star/introduction.html
	TArray<ATile*> Frontier;
	Frontier.Add(Start);
	TMap<ATile*, ATile*> CameFrom;
	CameFrom.Add(Start, nullptr);
	// from flow field
	while (!Frontier.IsEmpty())
	{
		ATile* Current = Frontier[0];
		Frontier.Remove(Current);
		if (Current == End)
		{
			// found target Tile
			break;
		}
		for (ATile* Next : Current->Neighbors)
		{
			if (Next && Next->IsWalkable(Affiliation) && !CameFrom.Contains(Next))
			{
				Frontier.Add(Next);
				CameFrom.Add(Next, Current);
			}
		}
	}
	// reconstruct Path
	if (!CameFrom.Contains(End))
	{
		return TArray<ATile*>();
	}
	ATile* Current = End;
	TArray<ATile*> Path;
	while (Current != Start)
	{
		Path.Add(Current);
		Current = CameFrom[Current];
	}
	return Path;
}

TArray<ATile*> ATileMap::GetPathToNearestAffiliatedBuilding(ATile* Start, EAffiliation Affiliation)
{
	//https://www.redblobgames.com/pathfinding/a-star/introduction.html
	TArray<ATile*> Frontier;
	Frontier.Add(Start);
	TMap<ATile*, ATile*> CameFrom;
	CameFrom.Add(Start, nullptr);
	ATile* Target = nullptr;
	// from flow field
	while (!Frontier.IsEmpty())
	{
		ATile* Current = Frontier[0];
		Frontier.Remove(Current);
		// check if Current Tile is a valid Target
		if (Current->Building
			&& Current->GetClaimant()
			&& Current->GetClaimant()->Affiliation == Affiliation
			&& Current->IsWalkable(Affiliation))
		{
			// found target Tile
			Target = Current;
			break;
		}
		// Add all Neighbors of Current tile to the Frontier, with Current Tile as CameFrom
		for (ATile* Next : Current->Neighbors)
		{
			if (Next && Next->IsWalkable(Affiliation) && !CameFrom.Contains(Next))
			{
				Frontier.Add(Next);
				CameFrom.Add(Next, Current);
			}
		}
	}
	// reconstruct Path
	TArray<ATile*> Path;
	if (!Target)
	{
		return Path;
	}
	ATile* Current = Target;
	while (Current != Start)
	{
		Path.Add(Current);
		Current = CameFrom[Current];
	}
	return Path;
}
