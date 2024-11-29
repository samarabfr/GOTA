// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ColonyBrainSettings.generated.h"

class UBuildingSettings;

UCLASS()
class GOTA_API UColonyBrainSettings : public UObject
{
	GENERATED_BODY()

	// ------------------- Replication Setup -------------------

	// ------------------- Init only Settings -------------------
private:
	UPROPERTY(EditDefaultsOnly)
	TArray<UBuildingSettings*> PossibleBuildings;

	UPROPERTY(EditDefaultsOnly)
	float FoodImportance = 1;

	UPROPERTY(EditDefaultsOnly)
	float FoodImportanceDescent = 0.1;

	UPROPERTY(EditDefaultsOnly)
	float WoodImportance = 1;

	UPROPERTY(EditDefaultsOnly)
	float WoodImportanceDescent = 0.1;

	UPROPERTY(EditDefaultsOnly)
	float StoneImportance = 1;

	UPROPERTY(EditDefaultsOnly)
	float StoneImportanceDescent = 0.1;

public:
	TArray<UBuildingSettings*> GetPossibleBuildings() const { return PossibleBuildings; }
	
	float GetFoodImportance() const { return FoodImportance; }
	float GetFoodImportanceDescent() const { return FoodImportanceDescent; }
	
	float GetWoodImportance() const { return WoodImportance; }
	float GetWoodImportanceDescent() const { return WoodImportanceDescent; }
	
	float GetStoneImportance() const { return StoneImportance; }
	float GetStoneImportanceDescent() const { return StoneImportanceDescent; }
};

// ------------------- Defaults Data Asset -------------------

UCLASS()
class GOTA_API UColonyBrainSettingsDefaults : public UPrimaryDataAsset
{
	GENERATED_BODY()
	UColonyBrainSettingsDefaults();

public:
	UPROPERTY(EditDefaultsOnly)
	UColonyBrainSettings* ColonyBrainSettings;
};
