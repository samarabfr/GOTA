// Fill out your copyright notice in the Description page of Project Settings.

#include "TileMap.h"
#include "FastNoiseWrapper.h"
#include "HexCoords.h"
#include "HexCoordsFunctions.h"
#include "TileGeneratedInfo.h"
#include "VectorTypes.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
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

void ATileMap::SpawnNewTile(FHexCoords Coords, float Height)
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

void ATileMap::GenerateTiles(int32 GenerationSize, int32 TileCount)
{
	// Generate Noisemap
	UFastNoiseWrapper* FastNoiseWrapper = nullptr;
	// Noise settings
	FastNoiseWrapper = NewObject<UFastNoiseWrapper>(this);
	const EFastNoise_NoiseType NoiseType = EFastNoise_NoiseType::Simplex;
	const int32 Seed = FMath::Rand32();
	const float Frequency = 0.1;
	const EFastNoise_Interp Interpolation = EFastNoise_Interp::Quintic;
	const EFastNoise_FractalType FractalType = EFastNoise_FractalType::FBM;
	const int32 Octaves = 1;
	const float Lacunarity = 2;
	const float gain = 0.5;
	const float CellularJitter = 0.45;
	const EFastNoise_CellularDistanceFunction CellularDistanceFunction = EFastNoise_CellularDistanceFunction::Euclidean;
	const EFastNoise_CellularReturnType CellularReturnType = EFastNoise_CellularReturnType::CellValue;
	// factors
	const float NoiseFactor = 500;
	// distance to middle point factor
	const FVector2d MiddlePoint = FVector2d(GenerationSize / 2, GenerationSize / 2);
	const float DistanceToMiddlePointMaxHeightFactor = 4000;
	const float DistanceToMiddlePointGradientFactor = 0.2;
	// Setup noise	
	FastNoiseWrapper->SetupFastNoise(NoiseType, Seed, Frequency, Interpolation, FractalType, Octaves,
	                                 Lacunarity, gain, CellularJitter, CellularDistanceFunction, CellularReturnType);
	// Generate Island
	TArray<FTileGeneratedInfo> GeneratedTiles;
	for (int32 Q = 0; Q < GenerationSize; ++Q)
	{
		for (int32 R = 0; R < GenerationSize; ++R)
		{
			const float DistanceToMiddlePointHeight = DistanceToMiddlePointMaxHeightFactor /
				FMath::Pow(EULERS_NUMBER,
					DistanceToMiddlePointGradientFactor * UE::Geometry::Distance(FVector2d(Q,R), MiddlePoint));
			const float NoiseHeight = NoiseFactor * FastNoiseWrapper->GetNoise2D(Q, R);
			const float Height = NoiseHeight + DistanceToMiddlePointHeight;
			GeneratedTiles.Add(FTileGeneratedInfo(FHexCoords(Q, R), Height, false, EBiome::Gras));
		}
	}
	// Cut to the right tile count

	// Initialize Tile Arrays to fit the island just right
	InitializeBothArrays(FHexCoords(GenerationSize, GenerationSize));
	// Spawn Tiles
	for (FTileGeneratedInfo GeneratedTile : GeneratedTiles)
	{
		SpawnNewTile(GeneratedTile.HexCoords, GeneratedTile.Height);
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
