// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "HexCoords.h"
#include "TerrainGeneratorDataAsset.h"
#include "Tile.h"
#include "GameFramework/Actor.h"
#include "TileMap.generated.h"


UCLASS()
class GOTA_API ATileMap : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	ATileMap();

public:
	void Init();
	void EnableTick();
	void MaxAllEcoValues();

private:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="TileMap")
	int32 TileTicksPerFrame;

	int32 IndexPosition = 0;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClass;

private:
	UPROPERTY()
	AGS_Ingame* GameState;

	UPROPERTY(Replicated)
	FHexCoords Size;

	// Array for replication to clients
	UPROPERTY(Replicated)
	TArray<ATile*> Tiles;

	// Array for fast access on server
	ATile** TilesArray;

	bool TryAddTile(FHexCoords HexCoords, ATile* Tile);

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<ATile*> ColonistsStarts;

	UPROPERTY(BlueprintReadOnly)
	TArray<ATile*> NativesStarts;

	UPROPERTY(EditDefaultsOnly)
	UTerrainGeneratorDataAsset* TerrainGenData;

	void InitializeBothArrays(FHexCoords SizeInit);

	ATile* SpawnNewTile(FHexCoords Coords, float Height);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="TileMap")
	ATile* GetTile(FHexCoords HexCoords);

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintAuthorityOnly, Category="TileMap")
	ATile* GetTileFast(FHexCoords HexCoords);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="TileMap")
	bool DoesTileExist(FHexCoords HexCoords);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="TileMap")
	ATile* GetRandomTile();

	TArray<ATile*> FindPathToNearestTile(ATile* Origin, EEntityType EntityType,
	                                     const std::function<bool(const ATile*)>& Condition) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="TileMap")
	TArray<ATile*> GetPath(ATile* Start, ATile* End);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileMap")
	TArray<ATile*>
	GetPathToNearestAffiliatedBuilding(ATile* Start, EAffiliation TargetAffiliation);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileMap")
	int32 TryReduceEcoValue(ASettlement* Initiator, EEcoValue EcoValue, int32 Amount, int32 Threshold, int32 MaxRange);

	void CountAllMaxEcoValues(int32& TotalMaxTrees, int32& TotalMaxWildlife, int32& TotalMaxForage);
};
