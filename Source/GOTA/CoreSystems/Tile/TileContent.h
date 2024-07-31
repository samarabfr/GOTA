#pragma once
#include "SpawnPointLayout.h"
#include "TileAssetSpawn.h"
#include "TileContent.generated.h"

class AGS_Ingame;
class ATile;

UCLASS()
class GOTA_API ATileContent : public AActor
{
	//Unreal Engine Mystery Code
	GENERATED_BODY()

public:
	ATileContent();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="TileContent")
	void OnEnteringActiveRangeOfGuardian();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="TileContent")
	void OnLeavingActiveRangeOfGuardian();

	void Init(ATile* Tile_);
	
private:
	UFUNCTION()
	void UpdateTrees(int32 Change);

	UFUNCTION()
	void UpdateForage(int32 Change);
	
	UFUNCTION()
	void ValidateBuildings();
	
	void SpawnProps();

	UPROPERTY()
	ATile* Tile;

	TArray<FTileAssetSpawn> TreeTileAssetSpawns;

	TArray<FTileAssetSpawn> PropTileAssetSpawns;

	TArray<FTileAssetSpawn> BuildingTileAssetSpawns;

	TArray<FTileAssetSpawn> ForageTileAssetSpawns;

	FTileAssetSpawn MainBuilding;

	void BringArrayToCorrectSize(TArray<FTileAssetSpawn>& Array, int32 Size);
	void SetSpawnPointsOnArray(TArray<FTileAssetSpawn>& Array, TArray<FSpawnPoint> SpawnPoints);
	
	UFUNCTION()
	void OnSpawnPointLayoutChanged();

	UFUNCTION()
	void ValidateAllTileAssets();
	
	UFUNCTION()
	void ValidateMainBuilding();
	
	void ValidateTileAssets(TArray<FTileAssetSpawn>& Array, const UDataTable* Assets);

	void SpawnTileAsset(FTileAssetSpawn& FTileAssetSpawn);

	void DespawnTileAsset(FTileAssetSpawn& FTileAssetSpawn);

	void FindRandomValidAssets(int32 Amount, const UDataTable* DataTable,
	                           TArray<FTileAsset*>& OutFoundAssets) const;

	UPROPERTY()
	AGS_Ingame* GameState;

	template <typename T>
	static void ShuffleTArray(TArray<T>& Array);
};
