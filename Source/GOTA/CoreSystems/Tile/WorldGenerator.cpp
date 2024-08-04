// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerator.h"

#include "VectorTypes.h"

void UWorldGenerator::Init(ATileMap* TileMap_, int32 TileCount_)
{
	TileMap = TileMap_;
	TileCount = TileCount_;
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
	CutAndInitializeArrays();
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
		for (FGeneratedTileInfo& GeneratedTile : GTiles)
		{
			GeneratedTile.IsLand = true;
			GeneratedTile.IsLandConnectedToMainIsland = false;
			GeneratedTile.IsWaterConnectedToOcean = false;
			GeneratedTile.Height = 0;
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
		Height += (FastNoiseWrapper->GetNoise2D(Tile.HexCoords.Q, Tile.HexCoords.R) + 1) / 2 * TerrainGenData->HeightNoiseParameter.NoiseFactor;
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
					TerrainGenData->MaxHeightDifference)
				{
					// adjust to Maxheightdifference. -1 extra to avoid infinite loops because of floating point errors
					GeneratedTile.Height = Neighbor->Height + TerrainGenData->MaxHeightDifference - 1;
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
	// Place beaches
	int32 TotalBeachCounter = 0;
	while (!Coast.IsEmpty() && TotalBeachCounter / static_cast<float>(Coast.Num()) < TerrainGenData->
		MinTotalBeachPercentage)
	{
		const int32 RandomIndex = FMath::RandRange(0, Coast.Num() - 1);
		int32 BeachCounter = 0;
		PlaceBeach(Coast[RandomIndex], BeachCounter);
		TotalBeachCounter += BeachCounter;
		// Check if there are still free slots for beaches to impede infinite loops
		bool HasOpenSlots = false;
		for (FGeneratedTileInfo* CoastTile : Coast)
		{
			if (CoastTile->OceanDistance == 1 && CoastTile->Height < TerrainGenData->HeightStep && CoastTile->Biome !=
				EBiome::Beach)
			{
				HasOpenSlots = true;
			}
		}
		if (!HasOpenSlots) break;
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
	int32 MountainCounterGoal = TerrainGenData->MountainMinPercentage * TileCount;
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

void UWorldGenerator::SpawnTiles()
{
	int32 SpawnCounter = 0;
	for (FGeneratedTileInfo Tile : GTiles)
	{
		if (Tile.IsLand)
		{
			// round to height steps
			float Height = FMath::TruncToFloat(Tile.Height  / TerrainGenData->HeightStep) * TerrainGenData->HeightStep;
			Height += TerrainGenData->HeightOffset;
			ATile* NewTile = TileMap->SpawnNewTile(Tile.HexCoords, Height);
			NewTile->SetBiome(Tile.Biome);
			++SpawnCounter;
		}
	}
}

void UWorldGenerator::CutAndInitializeArrays()
{
	// TODO: kleinstmöglichste Size berechnen
	TileMap->InitializeBothArrays(Size);
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

void UWorldGenerator::PlaceBeach(FGeneratedTileInfo* GeneratedTile, int32& BeachTileCounter)
{
	if (BeachTileCounter >= TerrainGenData->BeachSize
		|| !GeneratedTile
		|| GeneratedTile->OceanDistance != 1
		|| GeneratedTile->Biome == EBiome::Beach
		|| GeneratedTile->Height >= TerrainGenData->HeightStep)
		return;

	GeneratedTile->Biome = EBiome::Beach;
	++BeachTileCounter;
	for (FGeneratedTileInfo* Neighbor : GeneratedTile->Neighbors)
	{
		PlaceBeach(Neighbor, BeachTileCounter);
	}
}
