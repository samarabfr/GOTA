// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "GameResources.h"
#include "SettlementSettings.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "GameFramework/Actor.h"
#include "GOTA/CoreSystems/Faction/Building/Population.h"
#include "Settlement.generated.h"

class USettlementPopulation;
class UPopulationSettings;
class USettlementSettings;
class ACivilian;
class UBuilding;
class ATile;

UCLASS(Abstract, Blueprintable)
class ASettlement : public AActor
{
	GENERATED_BODY()

	// --------------------------- Replication Setup ---------------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// --------------------------- LifeCycle ---------------------------
protected:
	ASettlement();

	virtual void BeginPlay() override;

public:
	void S_Init(ATile* SpawnTile,
	            USettlementSettings* InSettlementSettings,
	            UPopulationSettings* InPopulationSettings);

	void EnableTick();

protected:
	virtual void Tick(float DeltaSeconds) override;

	// --------------------------- Utility ---------------------------
private:
	UPROPERTY(Replicated)
	USettlementSettings* Settings;

public:
	USettlementSettings* GetSettings() const { return Settings; }

	EAffiliation GetAffiliation() const { return Settings->GetAffiliation(); }

	FGameplayTagContainer GetGameplayTags() const { return Settings->GetGameplayTags(); }

	// --------------------------- Population ---------------------------
private:
	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	USettlementPopulation* Population;

	UPROPERTY()
	UPopulationSettings* PopulationSettings;

public:
	USettlementPopulation* GetPopulation() { return Population; }

	UPopulationSettings* GetPopulationSettings() { return PopulationSettings; }

	// --------------------------- Claims ---------------------------
protected:
	void RefreshBorderingUnclaimedTiles();

public:
	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	TArray<ATile*> ClaimedTiles;

	UPROPERTY()
	TArray<ATile*> BorderingUnclaimedTiles;

	bool IsBorderingUnclaimedTile(const ATile* Tile) const;

	// --------------------------- Building ---------------------------
public:
	void S_RegisterTile(ATile* Tile);
	void RegisterPopulation(UPopulation* InPopulation);

	void S_UnregisterTile(ATile* Tile);
	void UnregisterPopulation(UPopulation* InPopulation);

	// --------------------------- Resources ---------------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Settlement")
	FGameResources Resources;

	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	FGameResources PredictedProduction;

	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	FGameResources PredictedConsumption;

	void RegisterBuildingForResourcePrediction(UBuilding* Building);
	void UnregisterBuildingForResourcePrediction(UBuilding* Building);

	UFUNCTION()
	void UpdatePredictedProduction(const float Change, const EProductionType Type);

	UFUNCTION()
	void UpdatePredictedConsumption(const float Change, const EConsumptionType Type);

	UFUNCTION()
	void UpdatePredictionFromPopulation(int16 Change);

public:
	FGameResources GetResources() const { return Resources; }

	// Returns predicted production - predicted consumption
	FGameResources GetEffectivePredictedProduction() const { return PredictedProduction - PredictedConsumption; }

	// Returns raw predicted production
	FGameResources GetPredictedProduction() const { return PredictedProduction; }

	// Returns raw predicted consumption
	FGameResources GetPredictedConsumption() const { return PredictedConsumption; }

	void S_AddResources(FGameResources Amount);
	void S_RemoveResources(FGameResources Amount);
};
