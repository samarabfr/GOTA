// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementBalance.generated.h"

UCLASS(Blueprintable)
class GOTA_API USettlementBalance : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Claim")
	FRuntimeFloatCurve ClaimPrice;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Population")
	int32 PopulationGrowthThreshold;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="EcoValues")
	int32 NativeTreeThreshold;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="EcoValues")
	int32 NativeWildlifeThreshold;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="EcoValues")
	int32 NativeForageThreshold;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="EcoValues")
	float ForagingFoodToWoodRatio;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="EcoValues")
	int32 NativeMaxRange;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="EcoValues")
	int32 ColonistMaxRange;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army Spawning Condition")
	int32 MinimumPopulationToSpawnArmy;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army Spawning Condition")
	int32 HighPopulationThreshold;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army Spawning Condition")
	float HighPopulationImpact;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army Spawning Condition")
	float AggressiveMoodMaximumImpact;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army Spawning")
	float MinRatioOfEligiblePopJoiningArmy;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army Spawning")
	float MaxRatioOfEligiblePopJoiningArmy;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army Spawning")
	float BonusArmySizePerAngryPop;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army Spawning")
	int MinBuildingPopToJoinArmy;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army Spawning")
	int MinBuildingPopRemainingAfterJoining;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army Spawning")
	int ContentMoodPickBias;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army Spawning")
	int AngryMoodPickBias;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army Spawning")
	int FearMoodPickBias;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Importance Rating")
	float FoodImportance = 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Importance Rating")
	float FoodImportanceDescent = 0.1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Importance Rating")
	float WoodImportance = 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Importance Rating")
	float WoodImportanceDescent = 0.1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Importance Rating")
	float StoneImportance = 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Importance Rating")
	float StoneImportanceDescent = 0.1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Importance Rating")
	float WeaponsImportance = 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Importance Rating")
	float WeaponsImportanceDescent = 0.1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Importance Rating")
	float ShieldsImportance = 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Importance Rating")
	float ShieldsImportanceDescent = 0.1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Importance Rating")
	float HousingImportance = 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Importance Rating")
	float HousingImportanceDescent = 0.1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Importance Rating")
	float CurrentPopImportance = 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Importance Rating")
	float BuildingAlreadyExistsMalus = 3;
};
