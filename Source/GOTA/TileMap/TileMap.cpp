// Fill out your copyright notice in the Description page of Project Settings.

#include "TileMap.h"
#include "HexCoords.h"

FVector2D ATileMap::HexCoordsToWorldPos(FHexCoords HexCoords)
{
	const double X = HexCoords.Q * 1.5;
	const double Y = HexCoords.Q * 0.866 + HexCoords.R * 1.732;
	return FVector2D(X*GridSize,Y*GridSize);
}

FHexCoords ATileMap::WorldPosToHexCoords(FVector2D Vector)
{
	Vector = Vector/GridSize;
	const double FracQ =  0.667 * Vector.X;
	const double FracR = -0.333 * Vector.X + 0.577 * Vector.Y;
	const double FracS = -FracQ - FracR;
	const int32 RoundQ = round(FracQ);
	const int32 RoundR = round(FracR);
	const int32 RoundS = round(FracS);
	const int32 DiffQ = abs(RoundQ - FracQ);
	const int32 DiffR = abs(RoundR - FracR);
	const int32 DiffS = abs(RoundS - FracS);
	if(DiffQ > DiffR && DiffQ > DiffS)
	{
		return FHexCoords(-RoundR-RoundS,RoundR);
	}
	if(DiffR > DiffS)
	{
		return FHexCoords(RoundQ, -RoundQ-RoundS);
	}
	return FHexCoords(RoundQ,RoundR);
}

void ATileMap::Init(int32 Init_MapSize)
{
	MapSize = FMath::Max(Init_MapSize, 0);
	const int32 MapDiameter = MapSize * 2 + 1;
	TileMap.SetNum(MapDiameter * MapDiameter);
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
	TileMap.RemoveAt(Index);
	TileMap.Insert(Tile, Index);
	Tile->HexCoords = HexCoords;
}

ATile* ATileMap::Cpp_GetRandomTile()
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

TArray<ATile*> ATileMap::Cpp_GetNeighboringTiles(ATile* Origin)
{
	
	TRACE_CPUPROFILER_EVENT_SCOPE_STR("GetNeighbors");
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

TArray<ATile*> ATileMap::Cpp_GetPath(ATile* Start, ATile* End)
{
	TRACE_CPUPROFILER_EVENT_SCOPE_STR("GetPath");
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
		for (ATile* Next : Cpp_GetNeighboringTiles(Current))
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
