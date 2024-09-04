// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "EcoSystemDataAsset.generated.h"

UCLASS()
class GOTA_API UEcoSystemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// --------------------Max Modifiers-----------------------

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	TMap<EBiome, float> MaxForagePerBiome;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	float MaxForagePerMaxTree;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	float MaxWildlifePerForage;

	// -----------------GrowthThresholds-----------------------

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	float TreeGrowthThreshold;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	float WildlifeGrowthThreshold;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	float ForageGrowthThreshold;
	
	// ---------------------Base Growth------------------------

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	TMap<EBiome, float> BaseTreeGrowthPerBiome;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	TMap<EBiome, float> BaseWildlifeGrowthPerBiome;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	TMap<EBiome, float> BaseForageGrowthPerBiome;
	
	// ------------------Growth Modifiers----------------------

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	float TreeGrowthPerOwnTree;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	float TreeGrowthPerNeighborTree;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	float WildlifeGrowthPerOwnWildlife;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	float WildlifeGrowthPerNeighborWildlife;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	float ForageGrowthPerOwnForage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	float ForageGrowthPerNeighborForage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	float ForageGrowthPerOwnTree;
};
