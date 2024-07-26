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
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="TileContent")
	void OnSpawnPointLayoutChanged(FSpawnPointLayout NewSpawnPointLayout);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="TileContent")
	void OnEnteringActiveRangeOfGuardian();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="TileContent")
	void OnLeavingActiveRangeOfGuardian();
	
	void Init(const ATile* Tile);
};
