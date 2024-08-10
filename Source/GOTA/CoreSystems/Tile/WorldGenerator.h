// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "TerrainGeneratorDataAsset.h"
#include "NoiseParameter.h"
#include "CoreMinimal.h"
#include "GeneratedTileInfo.h"
#include "TileMap.h"
#include "WorldGenerator.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API UWorldGenerator : public UObject
{
	GENERATED_BODY()

public:
	void Init(ATileMap* TileMap_, int32 TileCount_, int32 ColonistsCount_, int32 NativesCount_);

	void GenerateWorld();

private:
	UPROPERTY()
	ATileMap* TileMap;

	int32 TileCount;

	FHexCoords Size;

	TArray<FGeneratedTileInfo> GTiles;

	FHexCoords SizeSpawn;

	TArray<FGeneratedTileInfo> GTilesSpawn;

	UPROPERTY()
	UTerrainGeneratorDataAsset* TerrainGenData;

	FGeneratedTileInfo* Middle;

	FGeneratedTileInfo* Origin;

	FGeneratedTileInfo* Volcano;

	UPROPERTY()
	UFastNoiseWrapper* FastNoiseWrapper;

	int8 MaxOceanDistance;

	int8 MaxVolcanoDistance;

	int32 ColonistsCount = 0;

	int32 NativesCount = 0;

	TArray<FGeneratedTileInfo*> ColonistsStarts;

	TArray<FGeneratedTileInfo*> NativesStarts;

	FGeneratedTileInfo* GetTile(const FHexCoords& Coords);

	FGeneratedTileInfo* GetTile(int32 Q, int32 R);

	void GenerateShape();

	void GenerateHeight();

	void CalculateOceanDistances();

	void CalculateVolcanoDistances();

	void ChooseVolcanoTile();

	void GenerateBeaches();

	void GenerateMountains();

	void GenerateRivers();

	void GenerateRiverPath(FGeneratedTileInfo* Tile, FGeneratedTileInfo* PrecedingTile,
	                       TArray<FGeneratedTileInfo*>& GeneratedPath, bool IsRiverBranch);

	bool MakesTooManyRiverConnections(FGeneratedTileInfo* Tile, TArray<FGeneratedTileInfo*>& GeneratedPath);

	bool HasUsedUpAllRiverConnections(FGeneratedTileInfo* Tile, TArray<FGeneratedTileInfo*>& GeneratedPath,
	                                  int8 MaxRiverConnections);

	void CalculateRiverDistances();

	bool HasOceanNeighbors(FGeneratedTileInfo* Tile);

	bool HasSearchedForTileAsNeighbor(FGeneratedTileInfo* Tile, FGeneratedTileInfo* SearchedForTile);

	bool HasRiverNeighbors(FGeneratedTileInfo* Tile);

	bool HasRiverSpringNeighbors(FGeneratedTileInfo* Tile);

	void SpawnTiles();

	void GenerateSpawnArray();

	void SetupNoise(FNoiseParameter& Parameter);

	void FlagConnectionToMainIsland(FGeneratedTileInfo* TileGeneratedInfo);

	void FlagConnectionToOcean(FGeneratedTileInfo* TileGeneratedInfo);

	void PlaceBeach(FGeneratedTileInfo* Tile, int32& BeachTileCounter, TArray<FGeneratedTileInfo*>& EligibleForBeach);

	void GenerateStartingPositions();

	void GenerateColonistsStartingPositions();

	void GenerateColonistsInitialStartingPositions();

	void FloodFillEveryTileWithColonistsDistances(FGeneratedTileInfo* Colonist);

	void GenerateColonistsFinalStartingPositions();

	float CalculateColonistStartScoreForTile(FGeneratedTileInfo* ColonistStart, FGeneratedTileInfo* Tile);

	void GenerateNativesStartingPositions();

	void GenerateNativesInitialStartingPositions();

	void GenerateNativesFinalStartingPositions();

	float CalculateNativesStartScoreForTile(FGeneratedTileInfo* NativesStart, FGeneratedTileInfo* Tile);
};
