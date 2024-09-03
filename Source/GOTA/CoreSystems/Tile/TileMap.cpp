// Fill out your copyright notice in the Description page of Project Settings.

#include "TileMap.h"
#include "HexCoords.h"
#include "HexCoordsFunctions.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "Misc/LowLevelTestAdapter.h"
#include "Net/UnrealNetwork.h"

void ATileMap::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATileMap, Tiles);
	DOREPLIFETIME(ATileMap, Size);
}

ATileMap::ATileMap()
{
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ATileMap::Init()
{
	for (ATile* Tile : Tiles)
	{
		if (Tile) Tile->ServerInit();
	}
}

void ATileMap::BeginPlay()
{
	Super::BeginPlay();

	GameState = GetWorld()->GetGameState<AGS_Ingame>();
}

void ATileMap::InitializeBothArrays(FHexCoords SizeInit)
{
	Size = SizeInit;
	TilesArray = new ATile*[Size.Q * Size.R];
	for (int32 Q = 0; Q < Size.Q; ++Q)
	{
		for (int32 R = 0; R < Size.R; ++R)
		{
			TilesArray[Q * Size.R + R] = nullptr;
			Tiles.Add(nullptr);
		}
	}
}

ATile* ATileMap::SpawnNewTile(FHexCoords Coords, float Height)
{
	FActorSpawnParameters SpawnInfo;
	FVector2d Vector2d = UHexCoordsFunctions::HexCoordsToVector2D(Coords);
	FVector Vector = FVector(Vector2d.X, Vector2d.Y, Height);
	ATile* NewTile = Cast<ATile>(GetWorld()->SpawnActor(TileClass.Get(), &Vector));
	if (NewTile)
	{
		if (TryAddTile(Coords, NewTile))
		{
			GameState->RegisterTileForTotalsUpdates(NewTile);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Tile couldnt be added to array (%d, %d)"), NewTile->HexCoords.Q,
			       NewTile->HexCoords.R)
			NewTile->Destroy();
		}
	}
	return NewTile;
}


bool ATileMap::TryAddTile(FHexCoords HexCoords, ATile* Tile)
{
	// Check for out of bounds
	if (HexCoords.Q >= Size.Q || HexCoords.R >= Size.R ||
		HexCoords.Q < 0 || HexCoords.R < 0)
	{
		return false;
	}
	if (DoesTileExist(HexCoords) || !Tile)
	{
		return false;
	}
	Tiles[HexCoords.Q * Size.R + HexCoords.R] = Tile;
	TilesArray[HexCoords.Q * Size.R + HexCoords.R] = Tile;
	Tile->HexCoords = HexCoords;
	// set neighbors on new tile
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
	return true;
}

ATile* ATileMap::GetTile(FHexCoords HexCoords)
{
	// Check for out of bounds
	if (HexCoords.Q >= Size.Q || HexCoords.R >= Size.R ||
		HexCoords.Q < 0 || HexCoords.R < 0)
	{
		return nullptr;
	}
	return Tiles[HexCoords.Q * Size.R + HexCoords.R];
}

ATile* ATileMap::GetTileFast(FHexCoords HexCoords)
{
	// Check for out of bounds
	if (HexCoords.Q >= Size.Q || HexCoords.R >= Size.R ||
		HexCoords.Q < 0 || HexCoords.R < 0)
	{
		return nullptr;
	}
	return TilesArray[HexCoords.Q * Size.R + HexCoords.R];
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
		const int32 RandomIndex = FMath::RandRange(0, Size.Q * Size.R - 1);
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
	ATile::bFreezeGrowthChanges = true;
	for (ATile* Tile : Tiles)
	{
		if (Tile) Tile->CalculateTurn();
	}
	ATile::bFreezeGrowthChanges = false;
	for (ATile* Tile : Tiles)
	{
		if (Tile)
		{
			Tile->CalculateTreeGrowthChange();
			Tile->CalculateWildlifeGrowthChange();
			Tile->CalculateForageChange();
			Tile->CalculatePopulationGrowthChange();
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

TArray<ATile*> ATileMap::GetPathToNearestAffiliatedBuilding(ATile* Start, EAffiliation TargetAffiliation,
                                                            EAffiliation Walker)
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
			&& Current->GetClaimant()->Affiliation == TargetAffiliation
			&& Current->IsWalkable(Walker))
		{
			// found target Tile
			Target = Current;
			break;
		}
		// Add all Neighbors of Current tile to the Frontier, with Current Tile as CameFrom
		for (ATile* Next : Current->Neighbors)
		{
			if (Next && Next->IsWalkable(Walker) && !CameFrom.Contains(Next))
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

int32 ATileMap::TryReduceEcoValue(ASettlement* Initiator, EEcoValue EcoValue, int32 Amount, int32 Threshold,
                                  int32 MaxRange)
{
	int32 _;
	int32 AmountReduced = 0;
	TArray<ATile*> Border;
	TArray<bool> AlreadyChecked;
	AlreadyChecked.SetNum(Tiles.Num());
	int32 ReducableCount = 0;
	int8 HighestCount = 0;
	int8 Range = 1;
	// fill Border initially
	for (ATile* ClaimedTile : Initiator->ClaimedTiles)
	{
		for (int32 i = 0; i < 6; ++i)
		{
			ATile* NeighborTile = ClaimedTile->Neighbors[i];
			bool Checked = NeighborTile
				               ? AlreadyChecked[ClaimedTile->Neighbors[i]->HexCoords.Q * Size.R
					               + ClaimedTile->Neighbors[i]->HexCoords.R]
				               : true;
			if (!Checked && !NeighborTile->Building)
			{
				Border.Add(NeighborTile);
				AlreadyChecked[ClaimedTile->Neighbors[i]->HexCoords.Q * Size.R
					+ ClaimedTile->Neighbors[i]->HexCoords.R] = true;
				int8 NeighborValue = 0;
				switch (EcoValue)
				{
				case EEcoValue::Tree:
					NeighborValue = NeighborTile->Trees->Current;
					break;
				case EEcoValue::Wildlife:
					NeighborValue = NeighborTile->Wildlife->Current;
					break;
				case EEcoValue::Forage:
					NeighborValue = NeighborTile->Forage->Current;
					break;
				}
				if (NeighborValue > Threshold)
					ReducableCount += NeighborValue - Threshold;
				if (NeighborValue > HighestCount)
					HighestCount = NeighborValue;
			}
		}
	}
	// loop through borders and reduce EcoValue in range until we are done
	while (Range < MaxRange && AmountReduced < Amount && !Border.IsEmpty())
	{
		int32 MissingCount = Amount - AmountReduced;
		// if Border has enough Trees to fill the request we have to reduce them equally on the border
		if (ReducableCount > MissingCount)
		{
			TArray<int8> TileValueReducedCount;
			TileValueReducedCount.SetNum(Border.Num());
			int8 TempThreshold = HighestCount;
			// figure out how much each tile has to be reduced
			// im not doing it every EcoValue one by one because it would trigger all OnEcoValueChange delegates
			// for every individual EcoValue, even though we probably reduce a bunch of them
			while (MissingCount > 0)
			{
				--TempThreshold;
				for (int32 i = 0; i < Border.Num(); ++i)
				{
					int8 BorderTileEcoValue = 0;
					switch (EcoValue)
					{
					case EEcoValue::Tree:
						BorderTileEcoValue = Border[i]->Trees->Current;
						break;
					case EEcoValue::Wildlife:
						BorderTileEcoValue = Border[i]->Wildlife->Current;
						break;
					case EEcoValue::Forage:
						BorderTileEcoValue = Border[i]->Forage->Current;
						break;
					}
					if (BorderTileEcoValue > TempThreshold)
					{
						++TileValueReducedCount[i];
						--MissingCount;
						if (MissingCount <= 0) break;
					}
				}
			}
			// Apply the EcoValue reduction
			for (int32 i = 0; i < Border.Num(); ++i)
			{
				switch (EcoValue)
				{
				case EEcoValue::Tree:
					Border[i]->Trees->Subtract(TileValueReducedCount[i], _);
					break;
				case EEcoValue::Wildlife:
					Border[i]->Wildlife->Subtract(TileValueReducedCount[i], _);
					break;
				case EEcoValue::Forage:
					Border[i]->Forage->Subtract(TileValueReducedCount[i], _);
					break;
				}
				AmountReduced += TileValueReducedCount[i];
			}
			return AmountReduced;
		}
		// Border has not enough EcoValue so we reduce all of the Border down to the Threshold
		for (ATile* BorderTile : Border)
		{
			int32 Value = 0;
			switch (EcoValue)
			{
			case EEcoValue::Tree:
				Value = BorderTile->Trees->Current;
				break;
			case EEcoValue::Wildlife:
				Value = BorderTile->Wildlife->Current;
				break;
			case EEcoValue::Forage:
				Value = BorderTile->Forage->Current;
				break;
			}
			if (Value > Threshold)
			{
				switch (EcoValue)
				{
				case EEcoValue::Tree:
					BorderTile->Trees->Subtract(Value - Threshold, _);
					break;
				case EEcoValue::Wildlife:
					BorderTile->Wildlife->Subtract(Value - Threshold, _);
					break;
				case EEcoValue::Forage:
					BorderTile->Forage->Subtract(Value - Threshold, _);
					break;
				}
				AmountReduced += Value - Threshold;
			}
		}
		// Refill Border with the next Range
		TArray<ATile*> NewBorder;
		for (ATile* BorderTile : Border)
		{
			for (int32 i = 0; i < 6; ++i)
			{
				ATile* NeighborTile = BorderTile->Neighbors[i];
				bool Checked = NeighborTile
					               ? AlreadyChecked[BorderTile->Neighbors[i]->HexCoords.Q * Size.R
						               + BorderTile->Neighbors[i]->HexCoords.R]
					               : true;
				if (!Checked && !NeighborTile->Building)
				{
					NewBorder.Add(NeighborTile);
					AlreadyChecked[BorderTile->Neighbors[i]->HexCoords.Q * Size.R
						+ BorderTile->Neighbors[i]->HexCoords.R] = true;
					int8 NeighborValue = 0;
					switch (EcoValue)
					{
					case EEcoValue::Tree:
						NeighborValue = NeighborTile->Trees->Current;
						break;
					case EEcoValue::Wildlife:
						NeighborValue = NeighborTile->Wildlife->Current;
						break;
					case EEcoValue::Forage:
						NeighborValue = NeighborTile->Forage->Current;
						break;
					}
					if (NeighborValue > Threshold)
						ReducableCount += NeighborValue - Threshold;
					if (NeighborValue > HighestCount)
						HighestCount = NeighborValue;
				}
			}
		}
		Border = NewBorder;
		++Range;
	}
	return AmountReduced;
}

void ATileMap::CountAllMaxEcoValues(int32& TotalMaxTrees, int32& TotalMaxWildlife, int32& TotalMaxForage)
{
	for (ATile* Tile : Tiles)
	{
		if (!Tile) continue;
		TotalMaxTrees += Tile->Trees->GetMaximum();
		TotalMaxWildlife += Tile->Wildlife->GetMaximum();
		TotalMaxForage += Tile->Forage->GetMaximum();
	}
}
