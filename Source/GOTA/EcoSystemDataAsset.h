// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EcoSystemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API UEcoSystemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	int32 StartingTrees;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	int32 MaxTrees;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	int32 TreeGrowthThreshold; 
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	int32 StartingForage;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	int32 MaxForage;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	int32 StartingWildlife;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	int32 MaxWildlife;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	int32 WildlifeGrowthThreshold; 
};
