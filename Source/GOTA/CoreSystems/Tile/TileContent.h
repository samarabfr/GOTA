#pragma once
#include "SpawnPointLayout.h"
#include "TileAssetSpawn.h"
#include "TileContent.generated.h"

class AGS_Ingame;
class ATile;

UCLASS()
class GOTA_API UTileContent : public UObject
{
	//Unreal Engine Mystery Code
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="TileContent")
	void OnEnteringActiveRangeOfGuardian();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="TileContent")
	void OnLeavingActiveRangeOfGuardian();

	void Init(ATile* Tile_, AGS_Ingame* GameState_);

	void SetRotation(FRotator Rotator);

private:
	void DespawnEverything();

	UFUNCTION()
	void UpdateTrees(int32 Change);

	UFUNCTION()
	void RedoTreeAssets();

	UFUNCTION()
	void UpdateForage(int32 Change);

	UFUNCTION()
	void ValidateBuildings(ATile* Tile_);

	void SpawnProps();

	UPROPERTY()
	ATile* Tile;

	TArray<FTileAssetSpawn> TreeTileAssetSpawns;

	TArray<FTileAssetSpawn> PropTileAssetSpawns;

	TArray<FTileAssetSpawn> BuildingTileAssetSpawns;

	TArray<FTileAssetSpawn> ForageTileAssetSpawns;

	FTileAssetSpawn MainBuilding;

	FRotator Rotation;

	void BringArrayToCorrectSize(TArray<FTileAssetSpawn>& Array, int32 Size);
	void SetSpawnPointsOnArray(TArray<FTileAssetSpawn>& Array, TArray<FSpawnPoint> SpawnPoints);

	UFUNCTION()
	void OnSpawnPointLayoutChanged();

	UFUNCTION()
	void ValidateEverything();

	UFUNCTION()
	void ValidateMainBuilding(ATile* Tile_);

	void ValidateTileAssets(TArray<FTileAssetSpawn>& Array, const UDataTable* Assets);

	void SpawnTileAsset(FTileAssetSpawn& FTileAssetSpawn);

	void DespawnTileAsset(FTileAssetSpawn& FTileAssetSpawn);

	void FindRandomValidAssets(int32 Amount, const UDataTable* DataTable,
	                           TArray<FTileAsset*>& OutFoundAssets) const;

	UPROPERTY()
	AGS_Ingame* GameState;

	void CalculateTransform(const FSpawnPoint& SpawnPoint, FTransform& Transform);
};
