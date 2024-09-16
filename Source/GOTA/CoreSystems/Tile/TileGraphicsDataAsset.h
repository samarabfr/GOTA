// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileAssetDA.h"
#include "Engine/DataAsset.h"
#include "TileGraphicsDataAsset.generated.h"

UCLASS()
class GOTA_API UTileGraphicsDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Biome Material Instances")
	UMaterialInstance* M_Grass;

	UPROPERTY(EditDefaultsOnly, Category="Biome Material Instances")
	UMaterialInstance* M_Beach;

	UPROPERTY(EditDefaultsOnly, Category="Biome Material Instances")
	UMaterialInstance* M_Mountain;

	UPROPERTY(EditDefaultsOnly, Category="Biome Material Instances")
	UMaterialInstance* M_Volcano;

	UPROPERTY(EditDefaultsOnly, Category="Claim Meshes")
	UStaticMesh* ClaimMesh;

	UPROPERTY(EditDefaultsOnly, Category="Claim Meshes")
	UStaticMesh* ClaimMeshRiver;

	UPROPERTY(EditDefaultsOnly, Category="Tile Asset")
	UTileAssetDA* DefaultTileAsset;
	
	UPROPERTY(EditDefaultsOnly, Category="Tile Asset")
	TArray<UTileAssetDA*> MainBuildingTileAssets;
	
	UPROPERTY(EditDefaultsOnly, Category="Tile Asset")
	TArray<UTileAssetDA*> BuildingTileAssets;

	UPROPERTY(EditDefaultsOnly, Category="Tile Asset")
	TArray<UTileAssetDA*> TreeTileAssets;

	UPROPERTY(EditDefaultsOnly, Category="Tile Asset")
	TArray<UTileAssetDA*> PropTileAssets;

	UPROPERTY(EditDefaultsOnly, Category="Tile Asset")
	TArray<UTileAssetDA*> ForageTileAssets;
	
	UPROPERTY(EditDefaultsOnly, Category="Tile Graphics")
	UDataTable* TileLayouts;
};
