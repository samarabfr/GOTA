// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PopulationSettings.generated.h"

UCLASS()
class GOTA_API UPopulationSettings : public UObject
{
	GENERATED_BODY()

	// ------------------- Replication Setup -------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool IsSupportedForNetworking() const override;

	// ------------------- Settings -------------------
	
private:
	// Per Second
	UPROPERTY(EditAnywhere, Replicated)
	float GrowthPerOwnPop = 0.0f;
	
	// Per Second
	UPROPERTY(EditAnywhere, Replicated)
	float GrowthPerNeighborPop = 0.0f;
	
public:
	float GetGrowthPerOwnPop() const { return GrowthPerOwnPop; }
	void S_SetGrowthPerOwnPop(float NewGrowthPerOwnPop);
	
	float GetGrowthPerNeighborPop() const { return GrowthPerNeighborPop; }
	void S_SetGrowthPerNeighborPop(float NewGrowthPerNeighborPop);
	
	UPROPERTY(VisibleInstanceOnly)
	bool IsStarving = false;
};

// ------------------- Defaults Data Asset -------------------

UCLASS()
class GOTA_API UPopulationSettingsDefaults : public UPrimaryDataAsset
{
	GENERATED_BODY()
	UPopulationSettingsDefaults();

public:
	UPROPERTY(EditDefaultsOnly)
	UPopulationSettings* PopulationSettings;
};
