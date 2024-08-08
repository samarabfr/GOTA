// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerator.h"
#include "HexCoordsFunctions.h"
#include "VectorTypes.h"

void UWorldGenerator::Init(ATileMap* TileMap_, int32 TileCount_, int32 ColonistsCount_, int32 NativesCount_)
{
	TileMap = TileMap_;
	TileCount = TileCount_;
	ColonistsCount = ColonistsCount_;
	NativesCount = NativesCount_;
	TerrainGenData = TileMap->TerrainGenData;
	Size.Q = FMath::Sqrt(TileCount / (TerrainGenData->LandToArraySizeRatio * 1.5));
	Size.R = Size.Q * 1.5;
	FastNoiseWrapper = NewObject<UFastNoiseWrapper>(this);
	// init GTiles
	GTiles.SetNum(Size.Q * Size.R);
	for (int32 Q = 0; Q < Size.Q; ++Q)
	{
		for (int32 R = 0; R < Size.R; ++R)
		{
			FGeneratedTileInfo* GeneratedTile = GetTile(Q, R);
			GeneratedTile->HexCoords = FHexCoords(Q, R);

			// Set neighbors on the new tile
			GeneratedTile->Neighbors[0] = GetTile(Q, R - 1);
			GeneratedTile->Neighbors[1] = GetTile(Q + 1, R - 1);
			GeneratedTile->Neighbors[2] = GetTile(Q + 1, R);
			GeneratedTile->Neighbors[3] = GetTile(Q, R + 1);
			GeneratedTile->Neighbors[4] = GetTile(Q - 1, R + 1);
			GeneratedTile->Neighbors[5] = GetTile(Q - 1, R);
		}
	}
	Middle = GetTile(Size.Q / 2, Size.R / 2);
	Origin = GetTile(0, 0);
}

void UWorldGenerator::GenerateWorld()
{
	GenerateShape();
	CalculateOceanDistances();
	ChooseVolcanoTile();
	CalculateVolcanoDistances();
	GenerateHeight();
	GenerateBeaches();
	GenerateMountains();
	GenerateRivers();
	GenerateStartingPositions();
	GenerateSpawnArray();
	SpawnTiles();
}

FGeneratedTileInfo* UWorldGenerator::GetTile(const FHexCoords& Coords)
{
	// out of bound check
	if (Coords.Q >= Size.Q && Coords.R >= Size.R && Coords.Q < 0 && Coords.R < 0) return nullptr;
	return &GTiles[Coords.Q * Size.R + Coords.R];
}

FGeneratedTileInfo* UWorldGenerator::GetTile(int32 Q, int32 R)
{
	// out of bounds check
	if (Q >= Size.Q || R >= Size.R || Q < 0 || R < 0) return nullptr;
	return &GTiles[Q * Size.R + R];
}

void UWorldGenerator::GenerateShape()
{
	int32 FilledCounter = 0;
	int32 Tries = 0;
	const int32 MaxTries = 100;
	// Generate shapes until it has enough tiles
	while (FilledCounter < TileCount && Tries < MaxTries)
	{
		FilledCounter = 0;
		++Tries;
		// reset every tile
		for (FGeneratedTileInfo& Tile : GTiles)
		{
			Tile.IsLand = true;
			Tile.IsLandConnectedToMainIsland = false;
			Tile.IsWaterConnectedToOcean = false;
			Tile.Height = 0;
		}
		const float MaxDistanceToMiddle = UE::Geometry::Distance(
			FVector2d(0, 0),
			FVector2d(Middle->HexCoords.Q, Middle->HexCoords.R));
		// Generate shape
		SetupNoise(TerrainGenData->ShapeNoiseParameter);
		for (FGeneratedTileInfo& Tile : GTiles)
		{
			const float DistanceToMiddlePoint = UE::Geometry::Distance(
				FVector2d(Tile.HexCoords.Q, Tile.HexCoords.R),
				FVector2d(Middle->HexCoords.Q, Middle->HexCoords.R));
			const float NormalizedDistance = DistanceToMiddlePoint / MaxDistanceToMiddle;
			float ShapeHeight = FastNoiseWrapper->GetNoise2D(Tile.HexCoords.Q, Tile.HexCoords.R) * TerrainGenData->
				ShapeNoiseParameter.NoiseFactor;
			ShapeHeight += TerrainGenData->DistanceToMiddlePointCurve.GetRichCurveConst()->Eval(NormalizedDistance)
				* TerrainGenData->ShapeDistanceToMiddlePointFactor;
			ShapeHeight += TerrainGenData->ShapeHeightOffset;
			if (ShapeHeight < TerrainGenData->IsLandThreshold)
				Tile.IsLand = false;
		}
		// cut away non-main islands
		FlagConnectionToMainIsland(Middle);
		for (FGeneratedTileInfo& Tile : GTiles)
		{
			if (!Tile.IsLandConnectedToMainIsland) Tile.IsLand = false;
		}
		// fill in oceans
		FlagConnectionToOcean(Origin);
		for (FGeneratedTileInfo& Tile : GTiles)
		{
			if (!Tile.IsWaterConnectedToOcean) Tile.IsLand = true;
		}
		// calculate filled tile count
		for (FGeneratedTileInfo& Tile : GTiles)
		{
			if (Tile.IsLand) ++FilledCounter;
		}
		// cut to tilecount
		// sort tiles by neigborcount
		TArray<TArray<FGeneratedTileInfo*>> TilesByNeighborCount;
		TilesByNeighborCount.SetNum(6);
		for (FGeneratedTileInfo& Tile : GTiles)
		{
			// count filled neighbors once before cutting and not while cutting
			int32 FilledNeighborsCounter = 0;
			for (int i = 0; i < 6; ++i)
			{
				if (Tile.Neighbors[i] && Tile.Neighbors[i]->IsLand) ++FilledNeighborsCounter;
			}
			// every filled tile should have 1-6 neighbors
			if (Tile.IsLand) TilesByNeighborCount[FilledNeighborsCounter - 1].Add(&Tile);
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
		// reset before checking non-main islands again
		for (FGeneratedTileInfo& Tile : GTiles)
		{
			Tile.IsLandConnectedToMainIsland = false;
		}
		// check for non-main islands again
		FlagConnectionToMainIsland(Middle);
		for (FGeneratedTileInfo& Tile : GTiles)
		{
			if (Tile.IsLand && !Tile.IsLandConnectedToMainIsland)
			{
				Tile.IsLand = false;
				--FilledCounter;
			}
		}
	}
}

void UWorldGenerator::GenerateHeight()
{
	// Calculate Height
	SetupNoise(TerrainGenData->HeightNoiseParameter);
	for (FGeneratedTileInfo& Tile : GTiles)
	{
		if (!Tile.IsLand) continue;

		float Height = 1;
		// Ocean distance
		const float NormalizedOceanDistance = static_cast<float>(Tile.OceanDistance) / MaxOceanDistance;
		Height += TerrainGenData->OceanDistanceCurve.GetRichCurveConst()->Eval(NormalizedOceanDistance)
			* TerrainGenData->OceanDistanceFactor;
		// volcano distance
		const float NormalizedVolcanoDistance = static_cast<float>(Tile.VolcanoDistance) /
			MaxVolcanoDistance;
		Height += TerrainGenData->VolcanoDistanceCurve.GetRichCurveConst()->Eval(NormalizedVolcanoDistance)
			* TerrainGenData->VolcanoDistanceFactor;
		// Noise map
		Height += (FastNoiseWrapper->GetNoise2D(Tile.HexCoords.Q, Tile.HexCoords.R) + 1) / 2 * TerrainGenData->
			HeightNoiseParameter.NoiseFactor;
		// Ocean distance as factor
		Height *= TerrainGenData->OceanDistanceCurve.GetRichCurveConst()->Eval(NormalizedOceanDistance);
		Tile.Height = Height;
	}
	// Apply max height difference and volcano height offset
	bool HeightChanged = true;
	// absolute value against user error
	float AbsoluteVolcanoOffsetToLowestNeighbor = FMath::Abs(TerrainGenData->VolcanoOffsetToLowestNeighbor);
	while (HeightChanged)
	{
		HeightChanged = false;
		for (FGeneratedTileInfo& GeneratedTile : GTiles)
		{
			// look for lowest neighbor and apply max height difference
			for (FGeneratedTileInfo* Neighbor : GeneratedTile.Neighbors)
			{
				if (GeneratedTile.VolcanoDistance > 0 && Neighbor && GeneratedTile.Height - Neighbor->Height >
					TerrainGenData->MaxHeightStepDifference * TerrainGenData->HeightStep)
				{
					// adjust to Maxheightdifference. -1 extra to avoid infinite loops because of floating point errors
					GeneratedTile.Height = Neighbor->Height + TerrainGenData->MaxHeightStepDifference * TerrainGenData->
						HeightStep - 1;
					HeightChanged = true;
				}
			}
		}
		// look for lowest neighbor and apply offset
		for (FGeneratedTileInfo* Neighbor : Volcano->Neighbors)
		{
			if (Neighbor && Neighbor->Height - Volcano->Height < AbsoluteVolcanoOffsetToLowestNeighbor)
			{
				Volcano->Height = Neighbor->Height - AbsoluteVolcanoOffsetToLowestNeighbor;
			}
		}
	}
}

void UWorldGenerator::CalculateOceanDistances()
{
	// Search Coast
	TArray<FGeneratedTileInfo*> OceanFrontier;
	for (FGeneratedTileInfo& Tile : GTiles)
	{
		if (Tile.IsLand)
		{
			// go through every neighbor
			for (int i = 0; i < 6; ++i)
			{
				// is bordering to water
				if (!Tile.Neighbors[i] || !Tile.Neighbors[i]->IsLand)
				{
					Tile.OceanDistance = 1;
					OceanFrontier.Add(&Tile);
					break; // No need to check other neighbors if bordering water
				}
			}
		}
	}
	// Flood fill
	int8 OceanDistance = 2;
	while (!OceanFrontier.IsEmpty())
	{
		TArray<FGeneratedTileInfo*> NewFrontier;
		for (FGeneratedTileInfo* FrontierTile : OceanFrontier)
		{
			for (int i = 0; i < 6; ++i)
			{
				if (FrontierTile->Neighbors[i] && FrontierTile->Neighbors[i]->OceanDistance == -1 && FrontierTile->
					Neighbors[i]->IsLand)
				{
					NewFrontier.Add(FrontierTile->Neighbors[i]);
					FrontierTile->Neighbors[i]->OceanDistance = OceanDistance;
				}
			}
		}
		++OceanDistance;
		OceanFrontier = NewFrontier;
	}
	MaxOceanDistance = OceanDistance - 1;
}

void UWorldGenerator::CalculateVolcanoDistances()
{
	TArray<FGeneratedTileInfo*> VolcanoFrontier;
	VolcanoFrontier.Add(Volcano);
	Volcano->VolcanoDistance = 0;
	// Flood fill
	int8 VolcanoDistance = 1;
	while (!VolcanoFrontier.IsEmpty())
	{
		TArray<FGeneratedTileInfo*> NewFrontier;
		for (FGeneratedTileInfo* FrontierTile : VolcanoFrontier)
		{
			for (int i = 0; i < 6; ++i)
			{
				if (FrontierTile->Neighbors[i] && FrontierTile->Neighbors[i]->VolcanoDistance == -1 && FrontierTile
					->Neighbors[i]->IsLand)
				{
					NewFrontier.Add(FrontierTile->Neighbors[i]);
					FrontierTile->Neighbors[i]->VolcanoDistance = VolcanoDistance;
				}
			}
		}
		++VolcanoDistance;
		VolcanoFrontier = NewFrontier;
	}
	MaxVolcanoDistance = VolcanoDistance - 1;
}

void UWorldGenerator::ChooseVolcanoTile()
{
	// calculate tiles that are eligible for being the volcano tile
	TArray<FGeneratedTileInfo*> TilesEligibleForVolcano;
	for (FGeneratedTileInfo& Tile : GTiles)
	{
		const float NormalizedOceanDistance = static_cast<float>(Tile.OceanDistance) / MaxOceanDistance;
		if (Tile.IsLand && NormalizedOceanDistance >= TerrainGenData->VolcanoSpawnOceanDistancePercentageThreshold)
		{
			TilesEligibleForVolcano.Add(&Tile);
		}
	}
	// Choose volcano tile
	if (!TilesEligibleForVolcano.IsEmpty())
	{
		const int32 RandomIndex = FMath::RandRange(0, TilesEligibleForVolcano.Num() - 1);
		Volcano = TilesEligibleForVolcano[RandomIndex];
		Volcano->Biome = EBiome::Volcano;
	}
}

void UWorldGenerator::GenerateBeaches()
{
	// Find coast
	TArray<FGeneratedTileInfo*> Coast;
	for (FGeneratedTileInfo& Tile : GTiles)
	{
		if (Tile.OceanDistance == 1)
			Coast.Add(&Tile);
	}
	// find only Eligible tiles
	TArray<FGeneratedTileInfo*> EligibleForBeach;
	for (FGeneratedTileInfo* Tile : Coast)
	{
		if (Tile->Height < TerrainGenData->HeightStep && Tile->Biome != EBiome::Beach)
			EligibleForBeach.Add(Tile);
	}
	// Place beaches
	int32 TotalBeachCounter = 0;
	int32 TotalBeachCounterGoal = Coast.Num() * TerrainGenData->MinTotalBeachPercentage;
	while (!EligibleForBeach.IsEmpty() && TotalBeachCounter <= TotalBeachCounterGoal)
	{
		const int32 RandomIndex = FMath::RandRange(0, EligibleForBeach.Num() - 1);
		int32 BeachCounter = 0;
		PlaceBeach(EligibleForBeach[RandomIndex], BeachCounter, EligibleForBeach);
		TotalBeachCounter += BeachCounter;
	}
}

void UWorldGenerator::GenerateMountains()
{
	// Copy GeneratedTiles array
	TArray<FGeneratedTileInfo*> GeneratedTilesHeightSorted;
	for (FGeneratedTileInfo& Tile : GTiles)
	{
		GeneratedTilesHeightSorted.Add(&Tile);
	}
	// Sort array
	GeneratedTilesHeightSorted.Sort([](const FGeneratedTileInfo& A, const FGeneratedTileInfo& B)
	{
		return A.Height > B.Height;
	});
	// set mountain biomes
	int32 MountainCounter = 0;
	int32 MountainCounterGoal = TerrainGenData->MinTotalMountainPercentage * TileCount;
	for (FGeneratedTileInfo* GeneratedTile : GeneratedTilesHeightSorted)
	{
		if (GeneratedTile->Biome != EBiome::Volcano
			&& MountainCounter < MountainCounterGoal)
		{
			GeneratedTile->Biome = EBiome::Mountain;
			++MountainCounter;
		}
	}
}

void UWorldGenerator::GenerateRivers()
{
	int32 LandTileCount = 0;
	for (FGeneratedTileInfo& Tile : GTiles)
	{
		if (Tile.IsLand) ++LandTileCount;
	}
	int32 TotalRiverCounter = 0;
	int32 TotalRiverCounterGoal = LandTileCount * TerrainGenData->MinTotalRiverPercentage;
	TArray<FGeneratedTileInfo*> EligibleRiverStartingTiles;
	// Find all possible ocean starting locations
	TArray<FGeneratedTileInfo*> EligibleOceanStartingTiles;
	for (FGeneratedTileInfo& Tile : GTiles)
	{
		if (Tile.OceanDistance == 1) EligibleOceanStartingTiles.Add(&Tile);
	}
	// Make Rivers
	while ((EligibleOceanStartingTiles.Num() > 0 || EligibleRiverStartingTiles.Num() > 0) && TotalRiverCounter <
		TotalRiverCounterGoal)
	{
		FGeneratedTileInfo* StartingTile = nullptr;
		bool IsOceanStart = FMath::RandRange(0.0, 0.99) < TerrainGenData->OceanStartingChance;
		// ocean start
		if (IsOceanStart)
		{
			if (EligibleOceanStartingTiles.IsEmpty()) continue;
			// Determine start location
			const int32 RandomIndex = FMath::RandRange(0, EligibleOceanStartingTiles.Num() - 1);
			StartingTile = EligibleOceanStartingTiles[RandomIndex];
			// Check if start location is valid
			if (!StartingTile || HasRiverNeighbors(StartingTile))
			{
				EligibleOceanStartingTiles.Remove(StartingTile);
				continue;
			}
		}
		// river branch start
		else
		{
			if (EligibleRiverStartingTiles.IsEmpty()) continue;
			// Determine start location
			const int32 RandomIndex = FMath::RandRange(0, EligibleRiverStartingTiles.Num() - 1);
			StartingTile = EligibleRiverStartingTiles[RandomIndex];
			// Check if start location is valid
			if (!StartingTile || StartingTile->HasRiverSpring)
			{
				EligibleRiverStartingTiles.Remove(StartingTile);
				continue;
			}
		}
		// Check for too many tries
		if (StartingTile->TriesAsStartPosition >= TerrainGenData->MaxTriesForRiverStartPositions)
		{
			if (EligibleOceanStartingTiles.Contains(StartingTile)) EligibleOceanStartingTiles.Remove(StartingTile);
			if (EligibleRiverStartingTiles.Contains(StartingTile)) EligibleRiverStartingTiles.Remove(StartingTile);
			continue;
		}
		StartingTile->TriesAsStartPosition++;
		if (!StartingTile) continue;
		// Make river path
		TArray<FGeneratedTileInfo*> RiverPath;
		GenerateRiverPath(StartingTile, nullptr, RiverPath, !IsOceanStart);
		// Check if path is not valid
		if (RiverPath.IsEmpty()
			|| (IsOceanStart && RiverPath.Num() < TerrainGenData->MinRiverLength)
			|| (!IsOceanStart && RiverPath.Num() < TerrainGenData->MinBranchLength)
			|| HasRiverNeighbors(RiverPath[RiverPath.Num() - 1]))
		{
			continue;
		}
		// set river stuff on tile
		for (FGeneratedTileInfo* Tile : RiverPath)
		{
			Tile->HasRiver = true;
			++TotalRiverCounter;
			EligibleRiverStartingTiles.Add(Tile);
		}
		RiverPath[RiverPath.Num() - 1]->HasRiverSpring = true;
		RiverPath[0]->HasRiverEnd = true;
	}
}

void UWorldGenerator::GenerateRiverPath(FGeneratedTileInfo* Tile, FGeneratedTileInfo* PrecedingTile,
                                        TArray<FGeneratedTileInfo*>& GeneratedPath, bool IsRiverBranch)
{
	if (!Tile) return;

	GeneratedPath.Add(Tile);
	// determine all neighbors that are eligible for the next tile
	TArray<FGeneratedTileInfo*> EligibleNextTiles;
	for (FGeneratedTileInfo* Neighbor : Tile->Neighbors)
	{
		if (Neighbor
			&& (!PrecedingTile || Neighbor != PrecedingTile)
			&& Neighbor->Height >= Tile->Height
			&& Neighbor->VolcanoDistance > 1
			&& !MakesTooManyRiverConnections(Neighbor, GeneratedPath)
			&& !HasOceanNeighbors(Neighbor)
			&& !HasSearchedForTileAsNeighbor(Neighbor, PrecedingTile)
			&& !HasRiverSpringNeighbors(Neighbor))
		{
			EligibleNextTiles.Add(Neighbor);
		}
	}
	// check if river generation is complete
	if (EligibleNextTiles.Num() == 0 || (!IsRiverBranch && HasRiverNeighbors(Tile)))
	{
		return;
	}
	// determine next tile
	const int32 RandomIndex = FMath::RandRange(0, EligibleNextTiles.Num() - 1);
	FGeneratedTileInfo* ChosenTile = EligibleNextTiles[RandomIndex];
	GenerateRiverPath(ChosenTile, Tile, GeneratedPath, IsRiverBranch);
}

bool UWorldGenerator::MakesTooManyRiverConnections(FGeneratedTileInfo* Tile, TArray<FGeneratedTileInfo*>& GeneratedPath)
{
	for (FGeneratedTileInfo* Neighbor : Tile->Neighbors)
	{
		if (!Neighbor || !Neighbor->HasRiver && !GeneratedPath.Contains(Neighbor)) continue;
		if (HasUsedUpAllRiverConnections(Neighbor, GeneratedPath, TerrainGenData->MaximumRiverConnections)) return true;
	}
	return false;
}

bool UWorldGenerator::HasUsedUpAllRiverConnections(FGeneratedTileInfo* Tile, TArray<FGeneratedTileInfo*>& GeneratedPath,
                                                   int8 MaxRiverConnections)
{
	// count every neighbor with a river
	int8 Counter = 0;
	for (FGeneratedTileInfo* Neighbor : Tile->Neighbors)
	{
		if (Neighbor && (Neighbor->HasRiver || GeneratedPath.Contains(Neighbor))) ++Counter;
	}
	return Counter >= MaxRiverConnections;
}

bool UWorldGenerator::HasOceanNeighbors(FGeneratedTileInfo* Tile)
{
	for (FGeneratedTileInfo* Neighbor : Tile->Neighbors)
	{
		if (!Neighbor || !Neighbor->IsLand) return true;
	}
	return false;
}

bool UWorldGenerator::HasSearchedForTileAsNeighbor(FGeneratedTileInfo* Tile, FGeneratedTileInfo* SearchedForTile)
{
	for (FGeneratedTileInfo* Neighbor : Tile->Neighbors)
	{
		if (Neighbor == SearchedForTile) return true;
	}
	return false;
}

bool UWorldGenerator::HasRiverNeighbors(FGeneratedTileInfo* Tile)
{
	for (FGeneratedTileInfo* Neighbor : Tile->Neighbors)
	{
		if (!Neighbor || Neighbor->HasRiver) return true;
	}
	return false;
}

bool UWorldGenerator::HasRiverSpringNeighbors(FGeneratedTileInfo* Tile)
{
	for (FGeneratedTileInfo* Neighbor : Tile->Neighbors)
	{
		if (!Neighbor || Neighbor->HasRiverSpring) return true;
	}
	return false;
}

void UWorldGenerator::SpawnTiles()
{
	int32 RiverCounter = 0; //TEMP for testing
	int32 Counter = 0; //TEMP for testing
	TileMap->InitializeBothArrays(SizeSpawn);
	for (FGeneratedTileInfo& Tile : GTilesSpawn)
	{
		if (Tile.IsLand)
		{
			// round to height steps
			float Height = FMath::TruncToFloat(Tile.Height / TerrainGenData->HeightStep) * TerrainGenData->HeightStep;
			Height += TerrainGenData->HeightOffset;
			ATile* NewTile = TileMap->SpawnNewTile(Tile.HexCoords, Height);
			NewTile->SetBiome(Tile.Biome);
			NewTile->SetIsRiver(Tile.HasRiver);
			++Counter;
			if (NewTile->bIsRiver) ++RiverCounter;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("River tiles: %d/%d, %f"), RiverCounter, Counter,
	       static_cast<float>(RiverCounter) / Counter)
}

void UWorldGenerator::GenerateSpawnArray()
{
	// Find the occupied area of the island
	int32 SmallestQ = Size.Q - 1;
	int32 SmallestR = Size.R - 1;
	int32 BiggestQ = 0;
	int32 BiggestR = 0;
	for (FGeneratedTileInfo& Tile : GTiles)
	{
		if (!Tile.IsLand) continue;
		if (SmallestQ > Tile.HexCoords.Q) SmallestQ = Tile.HexCoords.Q;
		if (SmallestR > Tile.HexCoords.R) SmallestR = Tile.HexCoords.R;
		if (BiggestQ < Tile.HexCoords.Q) BiggestQ = Tile.HexCoords.Q;
		if (BiggestR < Tile.HexCoords.R) BiggestR = Tile.HexCoords.R;
	}
	// Initialize new Spawn array
	SizeSpawn = FHexCoords(BiggestQ - SmallestQ + 1, BiggestR - SmallestR + 1);
	GTilesSpawn.SetNum(SizeSpawn.Q * SizeSpawn.R);
	// Copy island into spawn array
	for (int Q = SmallestQ; Q < BiggestQ + 1; ++Q)
	{
		for (int R = SmallestR; R < BiggestR + 1; ++R)
		{
			// copy Tiles because they have different HexCoords in the spawn array
			FGeneratedTileInfo Tile = *GetTile(Q, R);
			Tile.HexCoords = FHexCoords(Q - SmallestQ, R - SmallestR);
			GTilesSpawn[Tile.HexCoords.Q * SizeSpawn.R + Tile.HexCoords.R] = Tile;
		}
	}
}

void UWorldGenerator::SetupNoise(FNoiseParameter& Parameter)
{
	const int32 Seed = FMath::Rand32();
	FastNoiseWrapper->SetupFastNoise(Parameter.NoiseType, Seed, Parameter.Frequency, Parameter.Interpolation,
	                                 Parameter.FractalType, Parameter.Octaves, Parameter.Lacunarity, Parameter.gain,
	                                 Parameter.CellularJitter, Parameter.CellularDistanceFunction,
	                                 Parameter.CellularReturnType);
}

void UWorldGenerator::FlagConnectionToMainIsland(FGeneratedTileInfo* TileGeneratedInfo)
{
	if (!TileGeneratedInfo->IsLand) return;

	TileGeneratedInfo->IsLandConnectedToMainIsland = true;
	// Go through neighbors recursively
	for (FGeneratedTileInfo* Neighbor : TileGeneratedInfo->Neighbors)
	{
		if (Neighbor && !Neighbor->IsLandConnectedToMainIsland) FlagConnectionToMainIsland(Neighbor);
	}
}

void UWorldGenerator::FlagConnectionToOcean(FGeneratedTileInfo* TileGeneratedInfo)
{
	if (TileGeneratedInfo->IsLand) return;

	TileGeneratedInfo->IsWaterConnectedToOcean = true;
	// Go through neighbors recursively
	for (FGeneratedTileInfo* Neighbor : TileGeneratedInfo->Neighbors)
	{
		if (Neighbor && !Neighbor->IsWaterConnectedToOcean) FlagConnectionToOcean(Neighbor);
	}
}

void UWorldGenerator::PlaceBeach(FGeneratedTileInfo* Tile, int32& BeachTileCounter,
                                 TArray<FGeneratedTileInfo*>& EligibleForBeach)
{
	if (BeachTileCounter >= TerrainGenData->BeachSize
		|| !Tile
		|| Tile->OceanDistance != 1
		|| Tile->Biome == EBiome::Beach
		|| Tile->Height >= TerrainGenData->HeightStep)
		return;

	Tile->Biome = EBiome::Beach;
	++BeachTileCounter;
	EligibleForBeach.Remove(Tile);
	for (FGeneratedTileInfo* Neighbor : Tile->Neighbors)
	{
		PlaceBeach(Neighbor, BeachTileCounter, EligibleForBeach);
	}
}

void UWorldGenerator::GenerateStartingPositions()
{
	GenerateColonistsStartingPositions();
	GenerateNativesStartingPositions();
}

void UWorldGenerator::GenerateColonistsStartingPositions()
{
	GenerateColonistsInitialStartingPositions();
	GenerateColonistsFinalStartingPositions();
	// Apply to tiles
	for (FGeneratedTileInfo* Tile : ColonistsStarts)
	{
		Tile->IsColonistStart = true;
	}
}

void UWorldGenerator::GenerateColonistsInitialStartingPositions()
{
	// Find all coast tiles
	TArray<FGeneratedTileInfo*> CoastSorted;
	for (FGeneratedTileInfo& Tile : GTiles)
	{
		if (Tile.OceanDistance == 1) CoastSorted.Add(&Tile);
	}
	// Pick first colonist spawn randomly
	const int32 RandomIndex = FMath::RandRange(0, CoastSorted.Num() - 1);
	ColonistsStarts.Add(CoastSorted[RandomIndex]);
	// flood fill with distances
	FloodFillEveryTileWithColonistsDistances(CoastSorted[RandomIndex]);
	// Sort array
	CoastSorted.Sort([](const FGeneratedTileInfo& A, const FGeneratedTileInfo& B)
	{
		return A.ColonistsDistance > B.ColonistsDistance;
	});
	// pick all colonists spawns
	for (int i = 1; i < ColonistsCount; ++i)
	{
		// Pick best spot
		ColonistsStarts.Add(CoastSorted[0]);
		// flood fill with distances
		FloodFillEveryTileWithColonistsDistances(CoastSorted[0]);
		// Sort array
		CoastSorted.Sort([](const FGeneratedTileInfo& A, const FGeneratedTileInfo& B)
		{
			return A.ColonistsDistance > B.ColonistsDistance;
		});
	}
	// TEMP FOR TESTING
	for (FGeneratedTileInfo* Tile : ColonistsStarts)
	{
		Tile->Biome = EBiome::Mountain;
	}
}

void UWorldGenerator::GenerateColonistsFinalStartingPositions()
{
	// Find all coast tiles
	TArray<FGeneratedTileInfo*> Coast;
	for (FGeneratedTileInfo& Tile : GTiles)
	{
		if (Tile.OceanDistance == 1) Coast.Add(&Tile);
	}
	// Determine the positions through force calculations
	for (int _ = 0; _ < TerrainGenData->IterationsForceCalc; ++_)
	{
		// Compute forces
		TArray<FVector2D> Forces;
		Forces.SetNum(ColonistsCount);
		for (int i = 0; i < ColonistsCount; ++i)
		{
			for (int j = i + 1; j < ColonistsCount; ++j)
			{
				FVector2D Delta = UHexCoordsFunctions::HexCoordsToVector2D(ColonistsStarts[j]->HexCoords)
					- UHexCoordsFunctions::HexCoordsToVector2D(ColonistsStarts[i]->HexCoords);
				float Distance = UE::Geometry::Distance(FVector2D(0, 0), Delta);
				if (Distance > 0)
				{
					float ForceMagnitude = TerrainGenData->KConstantForceCalc / (Distance * Distance);
					FVector2D ForceVector = ForceMagnitude * (Delta / Distance);
					Forces[i] -= ForceVector;
					Forces[j] += ForceVector;
				}
			}
		}
		// apply forces
		for (int i = 0; i < ColonistsCount; ++i)
		{
			// Calculate unrestrained Position
			FVector2D UnrestrainedPosition =
				UHexCoordsFunctions::HexCoordsToVector2D(ColonistsStarts[i]->HexCoords)
				+ Forces[i] * TerrainGenData->DeltaTimeForceCalc;
			// find the closest allowed Tile
			FGeneratedTileInfo* ClosestTile = nullptr;
			float ClosestDistance = MAX_FLT;
			for (int j = 0; j < Coast.Num(); ++j)
			{
				FHexCoords Coords = Coast[j]->HexCoords;
				float Distance = UE::Geometry::Distance(UnrestrainedPosition,
				                                        UHexCoordsFunctions::HexCoordsToVector2D(Coords));
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					ClosestTile = Coast[j];
				}
			}
			if (ClosestTile != nullptr)
				ColonistsStarts[i] = ClosestTile;
		}
	}
	// TEMP FOR TESTING
	for (FGeneratedTileInfo* Tile : ColonistsStarts)
	{
		Tile->Biome = EBiome::Volcano;
	}
}

void UWorldGenerator::FloodFillEveryTileWithColonistsDistances(FGeneratedTileInfo* Colonist)
{
	if (!Colonist) return;
	TArray<FGeneratedTileInfo*> Frontier;
	Frontier.Add(Colonist);
	Colonist->ColonistsDistance = 0;
	int8 ColonistsDistance = 1;
	while (!Frontier.IsEmpty())
	{
		TArray<FGeneratedTileInfo*> NewFrontier;
		for (FGeneratedTileInfo* FrontierTile : Frontier)
		{
			for (int i = 0; i < 6; ++i)
			{
				if (FrontierTile->Neighbors[i] && FrontierTile->Neighbors[i]->IsLand && FrontierTile->Neighbors[i]->
					ColonistsDistance > ColonistsDistance)
				{
					NewFrontier.Add(FrontierTile->Neighbors[i]);
					FrontierTile->Neighbors[i]->ColonistsDistance = ColonistsDistance;
				}
			}
		}
		++ColonistsDistance;
		Frontier = NewFrontier;
	}
}

void UWorldGenerator::GenerateNativesStartingPositions()
{
}
