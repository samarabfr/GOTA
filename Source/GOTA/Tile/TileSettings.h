// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "TileSettings.generated.h"

UCLASS()
class GOTA_API UTileSettings : public UPrimaryDataAsset
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
	UStaticMesh* OceanLinesMesh;

	UPROPERTY(EditDefaultsOnly, Category="Claim Meshes")
	UStaticMesh* OceanLinesAtRiverDeltaMesh;

	UPROPERTY(EditDefaultsOnly, Category="Tile Content")
	FGameplayTag BuildingTag;

	UPROPERTY(EditDefaultsOnly, Category="Tile Content")
	FGameplayTag BuildingUnderConstructionTag;
	
	UPROPERTY(EditDefaultsOnly, Category="Tile Content")
	FGameplayTag BuildingDestroyedTag;
	
	UPROPERTY(EditDefaultsOnly, Category="Tile Graphics")
	UDataTable* TileLayouts;

	UPROPERTY(EditDefaultsOnly, Category="Tile Entities")
	TArray<FVector> CivilianSlots;
	
	UPROPERTY(EditDefaultsOnly, Category="Tile Entities")
	FVector ArmySlot;
};
