// Fill out your copyright notice in the Description page of Project Settings.

#include "TileMap.h"
#include "HexCoords.h"

void ATileMap::Init(int32 Init_MapSize)
{
	MapSize = FMath::Max(Init_MapSize, 0);
	const int32 MapDiameter = MapSize * 2 + 1;
	TileMap.SetNum(MapDiameter * MapDiameter, false);
}

bool ATileMap::DoesTileExist(FHexCoords HexCoords)
{
	if(GetTile(HexCoords))
	{
		return true;
	}
	return false;
}

ATile* ATileMap::GetTile(FHexCoords HexCoords)
{
	// Check for out of bounds
	if(	HexCoords.Q > MapSize	|| HexCoords.R > MapSize ||
		HexCoords.Q < -MapSize	|| HexCoords.R < -MapSize)
	{
		return nullptr;
	}
	// Check in 2D-Array
	const int32 Index = (HexCoords.Q + MapSize) * (MapSize * 2 + 1) + HexCoords.R + MapSize;
	return TileMap[Index];
}

void ATileMap::AddTile(FHexCoords HexCoords, ATile* Tile)
{
	if(DoesTileExist(HexCoords) || !Tile)
	{
		return;
	}
	const int32 Index = (HexCoords.Q + MapSize) * (MapSize * 2 + 1) + HexCoords.R + MapSize;
	TileMap[Index] = Tile;
	Tile->HexCoords = HexCoords;
}

ATile* ATileMap::GetRandomTile()
{
	// Try to get a Tile randomly
	const int32 MaxTries = 100;
	for (int i = 0; i < MaxTries; ++i)
	{
		const int32 RandomIndex = FMath::RandRange(0,(MapSize * 2 + 1) * (MapSize * 2 + 1) - 1);
		if(ATile* RandomTile = TileMap[RandomIndex])
		{
			return RandomTile;
		}
	}
	// Get the first Tile you can find in the TileMap 
	for (ATile* Tile : TileMap)
	{
		if(Tile)
		{
			return Tile;
		}
	}
	return nullptr;
}

TArray<ATile*> ATileMap::GetNeighboringTiles(ATile* Origin)
{
	TArray<ATile*> Neighbors;
	if(!Origin)
	{
		return Neighbors;
	}
	for (int Q = -1; Q <= 1; ++Q)
	{
		for (int R = -1; R <= 1; ++R)
		{
			FHexCoords NeighborCoords = Origin->HexCoords + FHexCoords(Q,R);
			if(Q!=R)
			{
				if(ATile* Neighbor = GetTile(NeighborCoords))
				{
					Neighbors.Add(Neighbor);
				}
			}
		}
	}
	return Neighbors;
}

TArray<ATile*> ATileMap::GetPath(ATile* Start, ATile* End)
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
		if(Current == End)
		{
			break;
		}
		for (ATile* Next : GetNeighboringTiles(Current))
		{
			if(Next->IsWalkable && !CameFrom.Contains(Next))
			{
				Frontier.Add(Next);
				CameFrom.Add(Next, Current);
			}
		}
	}
	// reconstruct Path
	if(!CameFrom.Contains(End))
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
