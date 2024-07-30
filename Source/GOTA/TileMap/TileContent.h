#pragma once
#include "SpawnPointLayout.h"
#include "TileAssetSpawn.h"
#include "TileContent.generated.h"

class AGOTAGameState;
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

	UFUNCTION()
	void UpdateTrees(int32 Change);

private:
	UPROPERTY()
	ATile* Tile;

	TArray<FTileAssetSpawn> TreeTileAssetSpawns;

	TArray<FTileAssetSpawn> PropTileAssetSpawn;

	TArray<FTileAssetSpawn> BuildingTileAssetSpawn;

	TArray<FTileAssetSpawn> ForageTileAssetSpawn;

	void BringArrayToCorrectSize(TArray<FTileAssetSpawn>& Array, int32 Size);
	void SetSpawnPointsOnArray(TArray<FTileAssetSpawn>& Array, TArray<FSpawnPoint> SpawnPoints);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="TileContent")
	UStaticMeshComponent* MainBuilding;

	UFUNCTION()
	void OnSpawnPointLayoutChanged();

	UFUNCTION()
	void ValidateAllTileAssets();

	void ValidateTrees();

private:
	void ValidateTileAssets(TArray<FTileAssetSpawn>& Array, const UDataTable* Assets);

	void SpawnTileAsset(FTileAssetSpawn& FTileAssetSpawn);

	void DespawnTileAsset(FTileAssetSpawn& FTileAssetSpawn);

	void FindRandomValidAssets(int32 Amount, const UDataTable* DataTable,
	                           TArray<FTileAsset*>& OutFoundAssets) const;
	
	UPROPERTY()
	AGOTAGameState* GameState;

	template<typename T>
	static void ShuffleTArray(TArray<T>& Array);
};
