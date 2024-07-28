#pragma once
#include "SpawnPointLayout.h"
#include "TileAsset.h"
#include "TileContent.generated.h"

class ATile;

UCLASS()
class GOTA_API ATileContent : public AActor
{
	//Unreal Engine Mystery Code
	GENERATED_BODY()

public:
	ATileContent();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="TileContent")
	void OnSpawnPointLayoutChanged(FSpawnPointLayout NewSpawnPointLayout);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="TileContent")
	void OnEnteringActiveRangeOfGuardian();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="TileContent")
	void OnLeavingActiveRangeOfGuardian();

	void Init(ATile* Tile_);

	UFUNCTION(BlueprintCallable, Category="TileContent")
	void UpdateTrees(int32 Change);

	UPROPERTY(BlueprintReadOnly, Category="TileContent")
	ATile* Tile;

	UPROPERTY(BlueprintReadOnly, Category="TileContent")
	TArray<UStaticMeshComponent*> TreeMeshes;

	UPROPERTY(BlueprintReadOnly, Category="TileContent")
	TArray<UStaticMeshComponent*> PropMeshes;

	UPROPERTY(BlueprintReadOnly, Category="TileContent")
	TArray<UStaticMeshComponent*> BuildingMeshes;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="TileContent")
	UDataTable* BuildingAssets;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="TileContent")
	UDataTable* TreeAssets;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="TileContent")
	UDataTable* PropAssets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="TileContent")
	UStaticMeshComponent* MainBuilding;

	UFUNCTION(BlueprintCallable, Category="TileContent")
	void UpdateBuildings();

	void FindRandomValidAssets(int32 Amount, const TArray<FTileAsset*>& Assets,
	                           TArray<FTileAsset*>& OutFoundAssets) const;
};
