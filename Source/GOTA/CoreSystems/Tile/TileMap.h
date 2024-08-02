// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FastNoiseWrapper.h"
#include "HexCoords.h"
#include "NoiseParameter.h"
#include "TerrainGeneratorDataAsset.h"
#include "Tile.h"
#include "TestRiverDataAsset.h"
#include "TestBiomeDataAsset.h"
#include "TileGeneratedInfo.h"
#include "GameFramework/Actor.h"
#include "TileMap.generated.h"

UCLASS()
class GOTA_API ATileMap : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	ATileMap();

	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClass;
	
	UPROPERTY(EditDefaultsOnly)
	UTerrainGeneratorDataAsset* TerrainGenData;
	
private:
	AGS_Ingame* GameState;
	
	UPROPERTY(Replicated)
	FHexCoords Size;
	
	// Array for replication to clients
	UPROPERTY(Replicated)
	TArray<ATile*> Tiles;
	
	// Array for fast access on server
	ATile** TilesArray;
	
	void InitializeBothArrays(FHexCoords SizeInit);

	void SpawnNewTile(FHexCoords Coords, float Height);
	
	bool TryAddTile(FHexCoords HexCoords, ATile* Tile);
	
	void SetupNoise(UFastNoiseWrapper* FastNoiseWrapper, FNoiseParameter& Parameter);
	
	void FlagConnectionToMainIsland(FTileGeneratedInfo* TileGeneratedInfo);
	
	void FlagConnectionToOcean(FTileGeneratedInfo* TileGeneratedInfo);
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="TileMap")
	ATile* GetTile(FHexCoords HexCoords);

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintAuthorityOnly, Category="TileMap")
	ATile* GetTileFast(FHexCoords HexCoords);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="TileMap")
	bool DoesTileExist(FHexCoords HexCoords);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="TileMap")
	ATile* GetRandomTile();
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileMap")
	void GenerateTiles(int32 TileCount);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileMap")
	void CalculateTurn();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="TileMap")
	TArray<ATile*> GetPath(ATile* Start, ATile* End, EAffiliation Affiliation);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileMap")
	TArray<ATile*> GetPathToNearestAffiliatedBuilding(ATile* Start, EAffiliation Affiliation);
};