// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileLayout.h"
#include "TileLayoutProvider.generated.h"

/**
 * Imports TileLayouts and SpawnLayouts from Assets and Provides them
 */
UCLASS()
class GOTA_API UTileLayoutProvider : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	// ---------------------------------------- Lifecycle ----------------------------------------
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// ---------------------------------------- Utility ----------------------------------------
private:
	TArray<FTileLayout> TileLayouts;

	void GenerateSpawnLayoutStructsFromDataAssets(FTileLayout& TileLayout);

public:
	FTileLayout* GetTileLayout(ETileLayout Layout);

	FTileLayout* GetFittingTileLayout(const FTerrain& Terrain);
};
