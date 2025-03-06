// Fill out your copyright notice in the Description page of Project Settings.

#include "TileMap.h"
#include "HexCoords.h"
#include "HexCoordsFunctions.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "Misc/LowLevelTestAdapter.h"
#include "Net/UnrealNetwork.h"
#include "Tile.h"
#include "GOTA/CoreSystems/Entity/Army.h"

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
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ATileMap::Init()
{
	for (ATile* Tile : Tiles)
	{
		if (!Tile) continue;
		// set neighbors on new tile
		FHexCoords HexCoords = Tile->HexCoords;
		Tile->Neighbors[0] = GetTileFast(FHexCoords(HexCoords.Q + 0, HexCoords.R - 1));
		Tile->Neighbors[1] = GetTileFast(FHexCoords(HexCoords.Q + 1, HexCoords.R - 1));
		Tile->Neighbors[2] = GetTileFast(FHexCoords(HexCoords.Q + 1, HexCoords.R - 0));
		Tile->Neighbors[3] = GetTileFast(FHexCoords(HexCoords.Q - 0, HexCoords.R + 1));
		Tile->Neighbors[4] = GetTileFast(FHexCoords(HexCoords.Q - 1, HexCoords.R + 1));
		Tile->Neighbors[5] = GetTileFast(FHexCoords(HexCoords.Q - 1, HexCoords.R + 0));
		Tile->S_Init();
	}
}

void ATileMap::EnableTick()
{
	SetActorTickEnabled(true);
}

void ATileMap::MaxAllEcoValues()
{
	for (ATile* Tile : Tiles)
	{
		if (!Tile) continue;
		Tile->EcoValues->MaxALlValues();
	}
}

void ATileMap::BeginPlay()
{
	Super::BeginPlay();

	GameState = GetWorld()->GetGameState<AGS_Ingame>();
}

void ATileMap::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	int32 CountTileTicked = 0;
	while (CountTileTicked < TileTicksPerFrame)
	{
		if (ATile* Tile = Tiles[IndexPosition])
		{
			Tile->GOTATick();
			++CountTileTicked;
		}
		++IndexPosition;
		if (IndexPosition >= Tiles.Num())
			IndexPosition = 0;
	}
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
	return true;
}

ATile* ATileMap::GetVolcanoTile()
{
	if (!VolcanoTile.IsValid())
	{
		for (ATile* Tile : Tiles)
		{
			if (Tile && Tile->GetTerrain().Biome == EBiome::Volcano)
			{
				VolcanoTile = Tile;
				return Tile;
			}
		}
	}
	return VolcanoTile.Get();
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

void ATileMap::CountAllMaxEcoValues(int32& TotalMaxTrees, int32& TotalMaxForage)
{
	for (ATile* Tile : Tiles)
	{
		if (!Tile) continue;
		TotalMaxTrees += Tile->EcoValues->GetMaxTrees();
		TotalMaxForage += Tile->EcoValues->GetMaxForage();
	}
}

void ATileMap::Delete()
{
	for (ATile* Tile : Tiles)
	{
		if (Tile)
		{
			Tile->Delete();
		}
	}
	if (HasAuthority())
	{
		Destroy();
	}
}

// -----------------  Tilefinding ------------------------


TMap<ATile*, int8> ATileMap::FindAllTilesWithRangesInRange(const TArray<ATile*>& Origin, int32 Range,
                                                           const EEntityType EntityType,
                                                           const std::function<bool(const ATile*)>& Condition) const
{
	TArray<int8> DistanceMap;
	TArray<ATile*> AllTiles = FindTilesInRange(Origin, DistanceMap, Range, EntityType, false, Condition);
	TMap<ATile*, int8> AllTilesWithRanges;
	for (ATile* Tile : AllTiles)
	{
		AllTilesWithRanges.Add(Tile, DistanceMap[Tile->HexCoords.Q * Size.R + Tile->HexCoords.R] - 1);
	}
	return AllTilesWithRanges;
}

ATile* ATileMap::FindNearestTile(const TArray<ATile*>& Origin, const EEntityType EntityType,
                                 const std::function<bool(const ATile*)>& Condition) const
{
	return FindNearestTileInRange(Origin, -1, EntityType, Condition);
}

ATile* ATileMap::FindNearestTileInRange(const TArray<ATile*>& Origin, int32 Range, const EEntityType EntityType,
                                        const std::function<bool(const ATile*)>& Condition) const
{
	TArray<int8> _;
	return FindNearestTileInRange(Origin, _, Range, EntityType, Condition);
}

ATile* ATileMap::FindNearestTileInRange(const TArray<ATile*>& Origin,
                                        TArray<int8>& OutDistanceMap, int32 Range, const EEntityType EntityType,
                                        const std::function<bool(const ATile*)>& Condition) const
{
	TArray<ATile*> FoundTargets = FindTilesInRange(Origin, OutDistanceMap, Range, EntityType, true, Condition);
	if (FoundTargets.IsEmpty()) return nullptr;
	return FoundTargets[FMath::RandRange(0, FoundTargets.Num() - 1)];
}

TArray<ATile*> ATileMap::FindTilesInRange(const TArray<ATile*>& Origin, TArray<int8>& OutDistanceMap, int32 Range,
                                          const EEntityType EntityType, bool bTerminateEarly,
                                          const std::function<bool(const ATile*)>& Condition) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE_STR("ATileMap::FindTilesInRange");
	if (Origin.IsEmpty()) return TArray<ATile*>();
	TArray<ATile*> Frontier = Origin;

	TArray<int8> DistanceMap;
	DistanceMap.SetNumZeroed(Tiles.Num()); // Distances need to seen as +1 because i cant do setnum with -1
	for (const ATile* FrontierTile : Frontier)
	{
		if (!FrontierTile)
			return TArray<ATile*>();
		DistanceMap[FrontierTile->HexCoords.Q * Size.R + FrontierTile->HexCoords.R] = 1;
	}
	TArray<ATile*> FoundTargets;
	int8 Distance = 2;
	// Find all frontier neighboring tiles
	// Frontier.IsEmpty() = flood fill finished
	// FoundTargets.IsEmpty() = terminate early as soon as targets have been found, because we only want the nearest
	// (optional) Distance - 1 <= Range => is in range
	while (!Frontier.IsEmpty() &&
		(!bTerminateEarly || FoundTargets.IsEmpty()) &&
		(Range < 0 || Distance <= Range + 1))
	{
		TArray<ATile*> NewFrontier;
		for (ATile* Current : Frontier)
		{
			// search for the next tiles
			for (ATile* Neighbor : Current->Neighbors)
			{
				// Neighbor => is ocean
				// (optional) Neighbor->AcceptsEntity(EntityType) => valid path for the entity type
				// DistanceMap[Neighbor->HexCoords.Q * Size.R + Neighbor->HexCoords.R] == 0 => has not been explored already
				if (Neighbor &&
					DistanceMap[Neighbor->HexCoords.Q * Size.R + Neighbor->HexCoords.R] == 0 &&
					(EntityType == EEntityType::None || Neighbor->AcceptsEntity(EntityType)))
				{
					NewFrontier.Add(Neighbor);
					DistanceMap[Neighbor->HexCoords.Q * Size.R + Neighbor->HexCoords.R] = Distance;
					// check if tile meets the conditions
					if (Condition(Neighbor))
					{
						FoundTargets.Add(Neighbor);
					}
				}
			}
		}
		++Distance;
		Frontier = NewFrontier;
	}
	OutDistanceMap = DistanceMap;
	return FoundTargets;
}

// -----------------  Pathfinding ------------------------

TArray<ATile*> ATileMap::FindPathToTile(const TArray<ATile*>& Origin, ATile* Target, EEntityType EntityType) const
{
	return FindPathToNearestTile(Origin, EntityType, [Target](const ATile* Tile)
	{
		return Tile == Target;
	});
}

TArray<ATile*> ATileMap::FindPathToNearestTile(const TArray<ATile*>& Origin, const EEntityType EntityType,
                                               const std::function<bool(const ATile*)>& Condition) const
{
	return FindPathToNearestTileInRange(Origin, -1, EntityType, Condition);
}

TArray<ATile*> ATileMap::FindPathToNearestTileFromSearchOrigin(const TArray<ATile*>& SearchOrigin,
                                                               const TArray<ATile*>& PathOrigin,
                                                               EEntityType EntityType,
                                                               const std::function<bool(const ATile*)>& Condition) const
{
	return FindPathToNearestTileInRangeFromSearchOrigin(SearchOrigin, PathOrigin, -1, EntityType, Condition);
}

TArray<ATile*> ATileMap::FindPathToNearestTileInRangeFromSearchOrigin(const TArray<ATile*>& SearchOrigin,
                                                                      const TArray<ATile*>& PathOrigin, int32 Range,
                                                                      EEntityType EntityType,
                                                                      const std::function<bool(const ATile*)>&
                                                                      Condition) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE_STR("ATileMap::FindPathToNearestTileInRangeFromSearchOrigin");
	if (SearchOrigin.IsEmpty() || PathOrigin.IsEmpty()) return TArray<ATile*>();
	return FindPathToTile(PathOrigin, FindNearestTileInRange(SearchOrigin, Range, EntityType, Condition), EntityType);
}

TArray<ATile*> ATileMap::FindPathToNearestTileInRange(const TArray<ATile*>& Origin, int32 Range,
                                                      EEntityType EntityType,
                                                      const std::function<bool(const ATile*)>& Condition) const
{
	TArray<int8> DistanceMap;
	ATile* Current = FindNearestTileInRange(Origin, DistanceMap, Range, EntityType, Condition);
	if (!Current)
		return TArray<ATile*>();
	TArray<ATile*> Path;
	while (!Origin.Contains(Current))
	{
		Path.Add(Current);
		int8 CurrentDistance = DistanceMap[Current->HexCoords.Q * Size.R + Current->HexCoords.R];
		TArray<ATile*> PossibleNextCurrents;
		for (ATile* Neighbor : Current->Neighbors)
		{
			if (!Neighbor) continue;
			int8 NeighborDistance = DistanceMap[Neighbor->HexCoords.Q * Size.R + Neighbor->HexCoords.R];
			if (NeighborDistance != 0 && NeighborDistance < CurrentDistance)
				PossibleNextCurrents.Add(Neighbor);
		}
		if (PossibleNextCurrents.IsEmpty())
			return TArray<ATile*>();
		Current = PossibleNextCurrents[FMath::RandRange(0, PossibleNextCurrents.Num() - 1)];
	}
	return Path;
}

// -----------------  TerrainGen ------------------------
