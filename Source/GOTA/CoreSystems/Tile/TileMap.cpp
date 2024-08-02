// Fill out your copyright notice in the Description page of Project Settings.

#include "TileMap.h"
#include "FastNoiseWrapper.h"
#include "HexCoords.h"
#include "HexCoordsFunctions.h"
#include "GeneratedTileInfo.h"
#include "VectorTypes.h"
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
	// init
	Tile->Init();
	return true;
}

void ATileMap::SetupNoise(UFastNoiseWrapper* FastNoiseWrapper, FNoiseParameter& Parameter)
{
	const int32 Seed = FMath::Rand32();
	FastNoiseWrapper->SetupFastNoise(Parameter.NoiseType, Seed, Parameter.Frequency, Parameter.Interpolation,
	                                 Parameter.FractalType, Parameter.Octaves, Parameter.Lacunarity, Parameter.gain,
	                                 Parameter.CellularJitter, Parameter.CellularDistanceFunction,
	                                 Parameter.CellularReturnType);
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

void ATileMap::GenerateTiles(int32 TileCount)
{
	// calculating Generation size. HAS TO BE CHANGED WHEN PARAMETERS CHANGE
	const float Approx = TerrainGenData->LandToArraySizeRatio;
	const int32 GenSizeQ = FMath::Sqrt(TileCount / (Approx * 1.5));
	const int32 GenSizeR = GenSizeQ * 1.5;
	UE_LOG(LogTemp, Warning, TEXT("Gensize is: %dx%d"), GenSizeQ, GenSizeR)
	// Calculate Middle Point
	const FVector2d MiddlePoint = FVector2d(GenSizeQ / 2, GenSizeR / 2);
	// Generate Noisemap
	UFastNoiseWrapper* FastNoiseWrapper = NewObject<UFastNoiseWrapper>(this);
	// Initialize Array
	TArray<FGeneratedTileInfo> GeneratedTiles;
	GeneratedTiles.SetNum(GenSizeQ * GenSizeR);
	// lambda function for bounds check
	auto IsInBounds = [&](FHexCoords Coords) -> bool
	{
		return Coords.Q < GenSizeQ && Coords.R < GenSizeR && Coords.Q >= 0 && Coords.R >= 0;
	};
	// lambda function for generated tile getting
	auto GetGeneratedTile = [&](FHexCoords Coords) -> FGeneratedTileInfo* {
		if (IsInBounds(Coords))
			return &GeneratedTiles[Coords.Q * GenSizeR + Coords.R];
		return nullptr;
	};
	// Init tiles
	for (int32 Q = 0; Q < GenSizeQ; ++Q)
	{
		for (int32 R = 0; R < GenSizeR; ++R)
		{
			FGeneratedTileInfo* GeneratedTile = GetGeneratedTile(FHexCoords(Q, R));
			GeneratedTile->HexCoords = FHexCoords(Q, R);

			// Set neighbors on the new tile
			GeneratedTile->Neighbors[0] = GetGeneratedTile(FHexCoords(Q, R - 1));
			GeneratedTile->Neighbors[1] = GetGeneratedTile(FHexCoords(Q + 1, R - 1));
			GeneratedTile->Neighbors[2] = GetGeneratedTile(FHexCoords(Q + 1, R));
			GeneratedTile->Neighbors[3] = GetGeneratedTile(FHexCoords(Q, R + 1));
			GeneratedTile->Neighbors[4] = GetGeneratedTile(FHexCoords(Q - 1, R + 1));
			GeneratedTile->Neighbors[5] = GetGeneratedTile(FHexCoords(Q - 1, R));
		}
	}
	// Generate Shape
	int32 FilledCounter = 0;
	int32 Tries = 0;
	const int32 MaxTries = 100;
	// Generate shapes until it has enough tiles
	while (FilledCounter < TileCount && Tries < MaxTries)
	{
		FilledCounter = 0;
		++Tries;
		// reset every tile
		for (auto& GeneratedTile : GeneratedTiles)
		{
			GeneratedTile.IsLand = true;
			GeneratedTile.IsLandConnectedToMainIsland = false;
			GeneratedTile.IsWaterConnectedToOcean = false;
			GeneratedTile.Height = 0;
		}
		const float MaxDistanceToMiddle = UE::Geometry::Distance(FVector2d(0, 0), MiddlePoint);
		SetupNoise(FastNoiseWrapper, TerrainGenData->ShapeNoiseParameter);
		for (int32 Q = 0; Q < GenSizeQ; ++Q)
		{
			for (int32 R = 0; R < GenSizeR; ++R)
			{
				const float DistanceToMiddlePoint = UE::Geometry::Distance(FVector2d(Q, R), MiddlePoint);
				const float NormalizedDistance = DistanceToMiddlePoint / MaxDistanceToMiddle;
				float ShapeHeight = FastNoiseWrapper->GetNoise2D(Q, R) * TerrainGenData->ShapeNoiseParameter.
					NoiseFactor;
				ShapeHeight += TerrainGenData->DistanceToMiddlePointCurve.GetRichCurveConst()->Eval(NormalizedDistance)
					* TerrainGenData->ShapeDistanceToMiddlePointFactor;
				ShapeHeight += TerrainGenData->ShapeHeightOffset;
				GeneratedTiles[Q * GenSizeR + R].Height = 1;
				if (ShapeHeight < TerrainGenData->ShouldGenerateThreshhold)
					GeneratedTiles[Q * GenSizeR + R].IsLand = false;
			}
		}
		// cut away non-main islands
		FlagConnectionToMainIsland(GetGeneratedTile(FHexCoords(MiddlePoint.X, MiddlePoint.Y)));
		for (auto& GeneratedTile : GeneratedTiles)
		{
			if (!GeneratedTile.IsLandConnectedToMainIsland) GeneratedTile.IsLand = false;
		}
		// fill in oceans
		FlagConnectionToOcean(GetGeneratedTile(FHexCoords(0, 0)));
		for (auto& GeneratedTile : GeneratedTiles)
		{
			if (!GeneratedTile.IsWaterConnectedToOcean) GeneratedTile.IsLand = true;
		}
		// calculate filled tile count
		for (auto& GeneratedTile : GeneratedTiles)
		{
			if (GeneratedTile.IsLand) ++FilledCounter;
		}
		// cut to tilecount
		// sort tiles by neigborcount
		TArray<TArray<FGeneratedTileInfo*>> TilesByNeighborCount;
		TilesByNeighborCount.SetNum(6);
		for (FGeneratedTileInfo& GeneratedTile : GeneratedTiles)
		{
			// count filled neighbors once before cutting and not while cutting
			int32 FilledNeighborsCounter = 0;
			for (int i = 0; i < 6; ++i)
			{
				if (GeneratedTile.Neighbors[i] && GeneratedTile.Neighbors[i]->IsLand) ++FilledNeighborsCounter;
			}
			// every filled tile should have 1-6 neighbors
			if (GeneratedTile.IsLand) TilesByNeighborCount[FilledNeighborsCounter - 1].Add(&GeneratedTile);
		}
		// cut until TileCount is met
		while (FilledCounter > TileCount && TileCount != 0)
		{
			// cut from the fewest neighbors to most. neighbors wont be recalculated to not smoothen too much
			for (int i = 0; i < 6; ++i)
			{
				if (TilesByNeighborCount[i].Num() > 0)
				{
					const int32 RandomIndex = FMath::RandRange(0, TilesByNeighborCount[i].Num() - 1);
					FGeneratedTileInfo* GeneratedTile = TilesByNeighborCount[i][RandomIndex];
					TilesByNeighborCount[i].RemoveAt(RandomIndex);
					GeneratedTile->IsLand = false;
					--FilledCounter;
					break;
				}
			}
		}
		// check for non-main islands again
		for (auto& GeneratedTile : GeneratedTiles)
		{
			GeneratedTile.IsLandConnectedToMainIsland = false;
		}
		FlagConnectionToMainIsland(GetGeneratedTile(FHexCoords(MiddlePoint.X, MiddlePoint.Y)));
		for (auto& GeneratedTile : GeneratedTiles)
		{
			if (GeneratedTile.IsLand && !GeneratedTile.IsLandConnectedToMainIsland)
			{
				GeneratedTile.IsLand = false;
				--FilledCounter;
			}
		}
	}
	// Check if shape generation failed
	if (Tries == MaxTries) UE_LOG(LogTemp, Warning, TEXT("Shape generation failed"))
	// Generate Height
	// Calculate distance to ocean
	// Search Coast
	TArray<FGeneratedTileInfo*> Frontier;
	for (FGeneratedTileInfo& GeneratedTile : GeneratedTiles)
	{
		if (GeneratedTile.IsLand)
		{
			// go through every neighbor
			for (int i = 0; i < 6; ++i)
			{
				// is bordering to water
				if (!GeneratedTile.Neighbors[i] || !GeneratedTile.Neighbors[i]->IsLand)
				{
					GeneratedTile.DistanceFromOcean = 1;
					Frontier.Add(&GeneratedTile);
					break; // No need to check other neighbors if bordering water
				}
			}
		}
	}
	// Flood fill
	int8 DistanceFromOcean = 2;
	while (!Frontier.IsEmpty())
	{
		TArray<FGeneratedTileInfo*> NewFrontier;
		for (FGeneratedTileInfo* FrontierTile : Frontier)
		{
			for (int i = 0; i < 6; ++i)
			{
				if (FrontierTile->Neighbors[i] && FrontierTile->Neighbors[i]->DistanceFromOcean == -1 && FrontierTile->
					Neighbors[i]->IsLand)
				{
					NewFrontier.Add(FrontierTile->Neighbors[i]);
					FrontierTile->Neighbors[i]->DistanceFromOcean = DistanceFromOcean;
				}
			}
		}
		++DistanceFromOcean;
		Frontier = NewFrontier;
	}
	const int8 MaxDistanceFromOcean = DistanceFromOcean - 1;
	// determine volcano tile
	// calculate tiles that are eligible for being the volcano tile
	TArray<FGeneratedTileInfo*> TilesEligibleForVolcano;
	for (FGeneratedTileInfo& GeneratedTile : GeneratedTiles)
	{
		if (GeneratedTile.IsLand && static_cast<float>(GeneratedTile.DistanceFromOcean) / MaxDistanceFromOcean >=
			TerrainGenData->VolcanoSpawnOceanDistancePercentageThreshold)
		{
			TilesEligibleForVolcano.Add(&GeneratedTile);
		}
	}
	// Choose volcano tile
	if (!TilesEligibleForVolcano.IsEmpty())
	{
		const int32 RandomIndex = FMath::RandRange(0, TilesEligibleForVolcano.Num() - 1);
		TilesEligibleForVolcano[RandomIndex]->Biome = EBiome::Volcano;
	}
	// Calculate Height
	SetupNoise(FastNoiseWrapper, TerrainGenData->HeightNoiseParameter);
	for (int32 Q = 0; Q < GenSizeQ; ++Q)
	{
		for (int32 R = 0; R < GenSizeR; ++R)
		{
			FGeneratedTileInfo* GeneratedTile = GetGeneratedTile(FHexCoords(Q, R));
			/*
			const float DistanceToMiddlePointHeight = TerrainGenData->DistanceToMiddlePointMaxHeightFactor /
				FMath::Pow(
					EULERS_NUMBER,
					TerrainGenData->DistanceToMiddlePointGradientFactor * UE::Geometry::Distance(
						FVector2d(Q, R), MiddlePoint));
			const float NoiseHeight = TerrainGenData->HeightNoiseParameter.NoiseFactor * FastNoiseWrapper->
				GetNoise2D(Q, R);
			float Height = NoiseHeight + DistanceToMiddlePointHeight + TerrainGenData->HeightOffset;
			*/

			// set height
			GeneratedTile->Height = 200 * GeneratedTile->DistanceFromOcean;
		}
	}
	// Initialize Tile Arrays to fit the island just right
	InitializeBothArrays(FHexCoords(GenSizeR, GenSizeR));
	// Spawn Tiles
	int32 SpawnCounter = 0;
	for (FGeneratedTileInfo GeneratedTile : GeneratedTiles)
	{
		if (GeneratedTile.IsLand)
		{
			ATile* NewTile = SpawnNewTile(GeneratedTile.HexCoords, GeneratedTile.Height);
			NewTile->SetBiome(GeneratedTile.Biome);
			++SpawnCounter;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Tiles spawned: %d"), SpawnCounter)
}

void ATileMap::FlagConnectionToMainIsland(FGeneratedTileInfo* TileGeneratedInfo)
{
	if (!TileGeneratedInfo->IsLand) return;

	TileGeneratedInfo->IsLandConnectedToMainIsland = true;
	// Go through neighbors recursively
	for (FGeneratedTileInfo* Neighbor : TileGeneratedInfo->Neighbors)
	{
		if (Neighbor && !Neighbor->IsLandConnectedToMainIsland) FlagConnectionToMainIsland(Neighbor);
	}
}

void ATileMap::FlagConnectionToOcean(FGeneratedTileInfo* TileGeneratedInfo)
{
	if (TileGeneratedInfo->IsLand) return;

	TileGeneratedInfo->IsWaterConnectedToOcean = true;
	// Go through neighbors recursively
	for (FGeneratedTileInfo* Neighbor : TileGeneratedInfo->Neighbors)
	{
		if (Neighbor && !Neighbor->IsWaterConnectedToOcean) FlagConnectionToOcean(Neighbor);
	}
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
