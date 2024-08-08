// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/Faction/Settlement/GameResources.h"
#include "GOTA/CoreSystems/Tile/TileAsset.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "BuildingTierData.generated.h"

USTRUCT(BlueprintType)
struct GOTA_API FBuildingTierData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	bool TierEnabled = false;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	FTileAsset MainBuildingAsset;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	FGameplayTagContainer GameplayTags;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	FGameResources Cost;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	int32 Housing = -1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	EProductionType ProductionType = EProductionType::MAX;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	int32 PopulationThreshold = -1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	int32 ProductionPerThreshold = -1;
};
