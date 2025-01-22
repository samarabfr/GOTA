#pragma once

#include "Terrain.h"
#include "TileAsset.h"
#include "TileAssetSpawn.h"
#include "TileContent.generated.h"

class UTileAssetProvider;
class AGS_Ingame;
class ATile;

UCLASS()
class GOTA_API UTileContent : public UObject
{
	//Unreal Engine Mystery Code
	GENERATED_BODY()
	
	TWeakObjectPtr<UTileAssetProvider> TileAssetProvider;
	
public:
	void Init(ATile* InTile, AGS_Ingame* InGameState);

	void SetRotation(FRotator Rotator);

	void OnSpawnPointLayoutChanged();

	void SetTerrain(const FTerrain& InTerrain);

private:
	void DespawnEveryTileAsset();

	UFUNCTION()
	void UpdateTrees(int32 Change);

	void NullEveryTileAsset();

	UFUNCTION()
	void UpdateForage(int32 Change);

	void UpdateBuildings();

	void UpdateProps();

	UPROPERTY()
	ATile* Tile;

	FTerrain Terrain;

	TArray<FTileAssetSpawn> TreeTileAssetSpawns;

	TArray<FTileAssetSpawn> PropTileAssetSpawns;

	TArray<FTileAssetSpawn> BuildingTileAssetSpawns;

	TArray<FTileAssetSpawn> ForageTileAssetSpawns;

	FTileAssetSpawn MainBuilding;

	// This is the Rotation of this TileContent instance, it is set by Tile whenever Tile gets rotated
	FRotator Rotation;

	void BringArrayToCorrectSize(TArray<FTileAssetSpawn>& Array, int32 Size);
	void SetSpawnPointsOnArray(TArray<FTileAssetSpawn>& Array, TArray<FSpawnPoint> SpawnPoints);
	
	void SetSpawnPointOnTileAssetSpawn(FTileAssetSpawn& TileAssetSpawn, const FSpawnPoint& SpawnPoint);
	
	UFUNCTION()
	void ValidateEverything();

	void ValidateTileAssets(TArray<FTileAssetSpawn>& Array, const TArray<UTileAsset*>& Assets);

	void ValidateMainBuildingAsset();
	
	void SpawnTileAsset(FTileAssetSpawn& TileAssetSpawn, const ESpawnState DesiredSpawnState = ESpawnState::Finished);


	void DespawnTileAsset(FTileAssetSpawn& FTileAssetSpawn);

	void FindRandomValidAssets(int32 Amount, const TArray<UTileAsset*>& AssetArray,
	                           TArray<UTileAsset*>& OutFoundAssets) const;

	UPROPERTY()
	AGS_Ingame* GameState;

	void CalculateTransform(const FSpawnPoint& SpawnPoint, FTransform& Transform);
};
