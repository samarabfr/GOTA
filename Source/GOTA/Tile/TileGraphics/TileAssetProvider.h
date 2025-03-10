// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TileAssetProvider.generated.h"

class UTileAsset;

UCLASS()
class GOTA_API UTileAssetProvider : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	// ---------------------------------------- Lifecycle ----------------------------------------
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// ---------------------------------------- Utility ----------------------------------------
private:
	UPROPERTY()
	UTileAsset* DefaultTileAsset;

	TArray<UTileAsset*> MainBuildingAssets;
	TArray<UTileAsset*> BuildingAssets;
	TArray<UTileAsset*> TreeAssets;
	TArray<UTileAsset*> PropAssets;
	TArray<UTileAsset*> ForageAssets;
	TArray<UTileAsset*> FoliageAssets;

public:
	UTileAsset* GetDefaultTileAsset() const { return DefaultTileAsset; }

	const TArray<UTileAsset*>& GetAllMainBuildingAssets() const { return MainBuildingAssets; }
	const TArray<UTileAsset*>& GetAllBuildingAssets() const { return BuildingAssets; }
	const TArray<UTileAsset*>& GetAllTreeAssets() const { return TreeAssets; }
	const TArray<UTileAsset*>& GetAllPropAssets() const { return PropAssets; }
	const TArray<UTileAsset*>& GetAllForageAssets() const { return ForageAssets; }
	const TArray<UTileAsset*>& GetAllFoliageAssets() const { return FoliageAssets; }
};
