// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "GameResources.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "GameFramework/Actor.h"
#include "GOTA/CoreSystems/Faction/Building/Population.h"
#include "Settlement.generated.h"

class UBuildingSettings;
class USettlementPopulation;
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
	void S_Init(ATile* SpawnTile);
	virtual void Delete();

	void EnableTick();

protected:
	virtual void Tick(float DeltaSeconds) override;

	// --------------------------- Utility ---------------------------
private:
	UPROPERTY(EditDefaultsOnly)
	EAffiliation Affiliation;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer GameplayTags;

public:
	EAffiliation GetAffiliation() const { return Affiliation; }

	FGameplayTagContainer GetGameplayTags() const { return GameplayTags; }

	// --------------------------- Population ---------------------------
private:
	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	USettlementPopulation* Population;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPopEatingPerSecondChangedSig, float, Change);

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing=OnRep_PopEatingPerSecond)
	float PopEatingPerSecond;

	UFUNCTION()
	void OnRep_PopEatingPerSecond(const float OldValue);

	UPROPERTY(EditAnywhere, Replicated)
	float StarvingThreshold;

	// Per Second
	UPROPERTY(EditAnywhere, Replicated)
	float GrowthPerOwnPop = 0.0f;

	// Per Second
	UPROPERTY(EditAnywhere, Replicated)
	float GrowthPerNeighborPop = 0.0f;

public:
	USettlementPopulation* GetPopulation() { return Population; }

	FOnPopEatingPerSecondChangedSig OnPopEatingPerSecondChanged;
	float GetPopEatingPerSecond() const { return PopEatingPerSecond; }
	void S_SetPopEatingPerSecond(float NewValue);

	float GetStarvingThreshold() const { return StarvingThreshold; }
	void S_SetStarvingThreshold(float NewValue);

	float GetGrowthPerOwnPop() const { return GrowthPerOwnPop; }

	float GetGrowthPerNeighborPop() const { return GrowthPerNeighborPop; }

	// --------------------------- Building ---------------------------
protected:
	void RefreshBorderingUnclaimedTiles();
	
	UPROPERTY(EditDefaultsOnly)
	TArray<UBuildingSettings*> StartingBuildings;

public:
	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	TArray<ATile*> ClaimedTiles;

	UPROPERTY()
	TArray<ATile*> BorderingUnclaimedTiles;

	bool IsBorderingUnclaimedTile(const ATile* Tile) const;

	void S_RegisterTile(ATile* Tile);
	void RegisterPopulation(UPopulation* InPopulation);

	void S_UnregisterTile(ATile* Tile);
	void UnregisterPopulation(UPopulation* InPopulation);
	int32 GetCountOfConstructionSites();

	// --------------------------- Resources ---------------------------
private:
	UPROPERTY(EditDefaultsOnly)
	FGameResources StartingResources;

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


	// --------------------------- Civilians ---------------------------

public:
	int32 GetCountOfBuilders();
};
