// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "TerrainGeneratorDataAsset.h"
#include "NoiseParameter.h"
#include "CoreMinimal.h"
#include "GeneratedTileInfo.h"
#include "WorldGenerator.generated.h"

class ATileMap;
/**
 * 
 */
UCLASS()
class GOTA_API UWorldGenerator : public UObject
{
	GENERATED_BODY()

public:
	void Init(ATileMap* TileMap_, int32 TileCount_);

	void GenerateWorld();

private:
	UPROPERTY()
	ATileMap* TileMap;

	int32 TileCount;

	FHexCoords Size;

	TArray<FGeneratedTileInfo> GTiles;

	TWeakObjectPtr<UTerrainGeneratorDataAsset> TerrainGenData;

	FGeneratedTileInfo* Middle;

	FGeneratedTileInfo* Origin;

	FGeneratedTileInfo* Volcano;

	UPROPERTY()
	UFastNoiseWrapper* FastNoiseWrapper;

	int8 MaxOceanDistance = 0;

	int8 MaxRiverDistance = 0;

	int8 MaxVolcanoDistance = 0;

	//-------------------------------------

	TArray<FGeneratedTileInfo*> Coast;

	TArray<FGeneratedTileInfo*> Land;

	FGeneratedTileInfo* GetTile(const FHexCoords& Coords);

	FGeneratedTileInfo* GetTile(int32 Q, int32 R);

	void GenerateShape();

	void FillArrays();

	void ReduceArraySizeToIslandSize();

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

	void SetupNoise(FNoiseParameter& Parameter);

	void FlagConnectionToMainIsland(FGeneratedTileInfo* TileGeneratedInfo);

	void FlagConnectionToOcean(FGeneratedTileInfo* TileGeneratedInfo);

	void PlaceBeach(FGeneratedTileInfo* Tile, int32& BeachTileCounter, TArray<FGeneratedTileInfo*>& EligibleForBeach);

	// -----------------Starting positions------------------
private:
	FGeneratedTileInfo* ColonistsStart;

	FGeneratedTileInfo* NativesStart;

	void GenerateStartingPositions();
	void GenerateInitialStartingPositions();
	void FloodFillColonistsDistances();
	void GenerateFinalStartingPositions();
	float CalculateStartingPositionScore(FGeneratedTileInfo* ScoredTile, FGeneratedTileInfo* OtherStartingPosition);

	// -----------------River connections------------------
private:
	void CalculateRiverConnections();
	void AddRiverConnectionsToOcean();
	void CleanupRiverConnections();
};
