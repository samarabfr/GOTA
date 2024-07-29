// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TileGraphicsDataAsset.generated.h"

UCLASS()
class GOTA_API UTileGraphicsDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Biome Material Instances")
	UMaterialInstance* M_Grass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Biome Material Instances")
	UMaterialInstance* M_Beach;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Biome Material Instances")
	UMaterialInstance* M_Mountain;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Biome Material Instances")
	UMaterialInstance* M_Volcano;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Asset DataTables")
	UDataTable* BuildingAssets;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Asset DataTables")
	UDataTable* TreeAssets;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Asset DataTables")
	UDataTable* PropAssets;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Asset DataTables")
	UDataTable* ForageAssets;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile Graphics")
	UDataTable* TileLayouts;
};
