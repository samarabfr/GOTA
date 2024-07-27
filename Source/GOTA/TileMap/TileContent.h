#pragma once
#include "SpawnPointLayout.h"
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

	UPROPERTY(BlueprintReadWrite, Category="TileContent")
	ATile* Tile;

	UPROPERTY(BlueprintReadOnly, Category="TileContent")
	TArray<UStaticMeshComponent*> TreeMeshes;
	
	UPROPERTY(BlueprintReadOnly, Category="TileContent")
	UStaticMeshComponent* MainBuilding;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="TileContent")
	void UpdateBuildings();
};
