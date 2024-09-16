// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GOTA/CoreSystems/Entity/Army.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingDataAsset.h"
#include "SettlementSettings.generated.h"

UCLASS(Blueprintable)
class GOTA_API USettlementSettings : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Colonist Settlement")
	TSubclassOf<AArmy> C_ArmyClass;

	UPROPERTY(EditDefaultsOnly, Category="Colonist Settlement")
	FGameplayTagContainer C_GameplayTags;
	
	UPROPERTY(EditDefaultsOnly, Category="Colonist Settlement")
	TArray<UBuildingDataAsset*> C_PossibleBuildings;
	
	UPROPERTY(EditDefaultsOnly, Category="Colonist Settlement")
	TArray<UBuildingDataAsset*> C_StartingBuildings;
	
	UPROPERTY(EditDefaultsOnly, Category="Colonist Settlement")
	FGameResources C_StartingResources;
	
	UPROPERTY(EditDefaultsOnly, Category="Native Settlement")
	TSubclassOf<AArmy> N_ArmyClass;

	UPROPERTY(EditDefaultsOnly, Category="Native Settlement")
	FGameplayTagContainer N_GameplayTags;

	UPROPERTY(EditDefaultsOnly, Category="Native Settlement")
	TArray<UBuildingDataAsset*> N_StartingBuildings;
	
	UPROPERTY(EditDefaultsOnly, Category="Native Settlement")
	FGameResources N_StartingResources;
	
	UPROPERTY(EditDefaultsOnly, Category="Population")
	int32 PopulationGrowthThreshold;

	UPROPERTY(EditDefaultsOnly, Category="EcoValues")
	int32 NativeTreeThreshold;

	UPROPERTY(EditDefaultsOnly, Category="EcoValues")
	int32 NativeWildlifeThreshold;

	UPROPERTY(EditDefaultsOnly, Category="EcoValues")
	int32 NativeForageThreshold;

	UPROPERTY(EditDefaultsOnly, Category="EcoValues")
	float ForagingFoodToWoodRatio;

	UPROPERTY(EditDefaultsOnly, Category="EcoValues")
	int32 NativeMaxRange;

	UPROPERTY(EditDefaultsOnly, Category="EcoValues")
	int32 ColonistMaxRange;

	UPROPERTY(EditDefaultsOnly, Category="Army Spawning Condition")
	int32 MinimumPopulationToSpawnArmy;

	UPROPERTY(EditDefaultsOnly, Category="Army Spawning Condition")
	int32 HighPopulationThreshold;

	UPROPERTY(EditDefaultsOnly, Category="Army Spawning Condition")
	float HighPopulationImpact;

	UPROPERTY(EditDefaultsOnly, Category="Army Spawning Condition")
	float AggressiveMoodMaximumImpact;

	UPROPERTY(EditDefaultsOnly, Category="Army Spawning")
	float MinRatioOfEligiblePopJoiningArmy;

	UPROPERTY(EditDefaultsOnly, Category="Army Spawning")
	float MaxRatioOfEligiblePopJoiningArmy;

	UPROPERTY(EditDefaultsOnly, Category="Army Spawning")
	float BonusArmySizePerAngryPop;

	UPROPERTY(EditDefaultsOnly, Category="Army Spawning")
	int MinBuildingPopToJoinArmy;

	UPROPERTY(EditDefaultsOnly, Category="Army Spawning")
	int MinBuildingPopRemainingAfterJoining;

	UPROPERTY(EditDefaultsOnly, Category="Army Spawning")
	int ContentMoodPickBias;

	UPROPERTY(EditDefaultsOnly, Category="Army Spawning")
	int AngryMoodPickBias;

	UPROPERTY(EditDefaultsOnly, Category="Army Spawning")
	int FearMoodPickBias;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float FoodImportance = 1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float FoodImportanceDescent = 0.1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float WoodImportance = 1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float WoodImportanceDescent = 0.1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float StoneImportance = 1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float StoneImportanceDescent = 0.1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float WeaponsImportance = 1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float WeaponsImportanceDescent = 0.1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float ShieldsImportance = 1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float ShieldsImportanceDescent = 0.1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float HousingImportance = 1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float HousingImportanceDescent = 0.1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float CurrentPopImportance = 1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float BuildingAlreadyExistsMalus = 3;
};
