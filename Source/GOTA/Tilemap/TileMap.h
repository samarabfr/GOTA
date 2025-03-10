// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>

#include "GameFramework/Actor.h"
#include "GOTA/Tile/HexCoords.h"
#include "GOTA/Utility/Enums.h"
#include "TileMap.generated.h"

class UTerrainGeneratorDataAsset;
class ATile;
class AGS_Ingame;

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
	void Delete();

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

	TWeakObjectPtr<ATile> VolcanoTile;

	// Array for fast access on server
	ATile** TilesArray;

	bool TryAddTile(FHexCoords HexCoords, ATile* Tile);

public:
	ATile* GetVolcanoTile();
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

	void CountAllMaxEcoValues(int32& TotalMaxTrees, int32& TotalMaxForage);


	// -----------------  Tilefinding -----------------------
public:
	TMap<ATile*, int8> FindAllTilesWithRangesInRange(const TArray<ATile*>& Origin,
	                                                 int32 Range = -1,
	                                                 const EEntityType EntityType = EEntityType::None,
	                                                 const std::function<bool(const ATile*)>& Condition = [
		                                                 ](const ATile*)
	                                                 {
		                                                 return true;
	                                                 }) const;
	ATile* FindNearestTile(const TArray<ATile*>& Origin,
	                       EEntityType EntityType = EEntityType::None,
	                       const std::function<bool(const ATile*)>& Condition = [](const ATile*)
	                       {
		                       return true;
	                       }) const;
	ATile* FindNearestTileInRange(const TArray<ATile*>& Origin,
	                              int32 Range = -1,
	                              const EEntityType EntityType = EEntityType::None,
	                              const std::function<bool(const ATile*)>& Condition = [](const ATile*)
	                              {
		                              return true;
	                              }) const;

private:
	ATile* FindNearestTileInRange(const TArray<ATile*>& Origin,
	                              TArray<int8>& OutDistanceMap,
	                              int32 Range = -1,
	                              const EEntityType EntityType = EEntityType::None,
	                              const std::function<bool(const ATile*)>& Condition = [](const ATile*)
	                              {
		                              return true;
	                              }) const;
	TArray<ATile*> FindTilesInRange(const TArray<ATile*>& Origin,
	                                TArray<int8>& OutDistanceMap,
	                                int32 Range = -1,
	                                const EEntityType EntityType = EEntityType::None,
	                                bool bTerminateEarly = false,
	                                const std::function<bool(const ATile*)>& Condition = [](const ATile*)
	                                {
		                                return true;
	                                }) const;

	// -----------------  Pathfinding ------------------------
public:
	TArray<ATile*> FindPathToTile(const TArray<ATile*>& Origin, ATile* Target,
	                              EEntityType EntityType = EEntityType::None) const;
	TArray<ATile*> FindPathToNearestTile(const TArray<ATile*>& Origin, EEntityType EntityType = EEntityType::None,
	                                     const std::function<bool(const ATile*)>& Condition = [
		                                     ](const ATile*)
	                                     {
		                                     return true;
	                                     }) const;

	TArray<ATile*> FindPathToNearestTileFromSearchOrigin(const TArray<ATile*>& SearchOrigin,
	                                                     const TArray<ATile*>& PathOrigin,
	                                                     EEntityType EntityType = EEntityType::None,
	                                                     const std::function<bool(const ATile*)>& Condition = [
		                                                     ](const ATile*)
	                                                     {
		                                                     return true;
	                                                     }) const;
	TArray<ATile*> FindPathToNearestTileInRangeFromSearchOrigin(const TArray<ATile*>& SearchOrigin,
	                                                            const TArray<ATile*>& PathOrigin,
	                                                            int32 Range = -1,
	                                                            EEntityType EntityType = EEntityType::None,
	                                                            const std::function<bool(const ATile*)>& Condition = [
		                                                            ](const ATile*)
	                                                            {
		                                                            return true;
	                                                            }) const;
	TArray<ATile*> FindPathToNearestTileInRange(const TArray<ATile*>& Origin, int32 Range,
	                                            EEntityType EntityType = EEntityType::None,
	                                            const std::function<bool(const ATile*)>& Condition = [
		                                            ](const ATile*)
	                                            {
		                                            return true;
	                                            }) const;

	// -----------------  TerrainGen ------------------------
private:
	UPROPERTY(EditDefaultsOnly)
	UTerrainGeneratorDataAsset* TerrainGenData;

	TWeakObjectPtr<ATile> ColonistsStart;
	TWeakObjectPtr<ATile> NativesStart;

public:
	UTerrainGeneratorDataAsset* GetTerrainGenData() const { return TerrainGenData; }
	TWeakObjectPtr<ATile> GetColonistsStart() const { return ColonistsStart; }
	void SetColonistsStart(TWeakObjectPtr<ATile> NewColonistsStart) { ColonistsStart = NewColonistsStart; }
	TWeakObjectPtr<ATile> GetNativesStart() const { return NativesStart; }
	void SetNativesStart(TWeakObjectPtr<ATile> NewNativesStart) { NativesStart = NewNativesStart; }
};
