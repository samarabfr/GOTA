// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GOTA/CoreSystems/Faction/Settlement/GameResources.h"
#include "GOTA/CoreSystems/Utility/Enums.h"

#include "Building.generated.h"

class ASettlement;
class ATile;
class UPopulation;
class ACivilian;
class UBuildingSettings;

UCLASS(Blueprintable)
class GOTA_API UBuilding : public UObject
{
	GENERATED_BODY()
	// ------------------------------------ Replication Setup --------------------------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	// ---------------------------------------- Lifecycle ----------------------------------------
protected:
	UBuilding();

public:
	void ServerInit(UBuildingSettings* InSettings, ATile* InTile, ASettlement* InSettlement);
	void ClientInit();

	void ServerTick(float DeltaSeconds);
	void ClientTick(const float DeltaSeconds);

private:
	virtual void BeginDestroy() override;

	// ---------------------------------------- Utility ----------------------------------------
public:
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Building")
	UBuildingSettings* Settings;

	UPROPERTY(Replicated)
	ATile* Tile;

	UPROPERTY(Replicated)
	ASettlement* Settlement;

	// --------------------------------------- Population ---------------------------------------
public:
	UPopulation* GetPopulation() const { return Population; }

private:
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Building")
	UPopulation* Population;

	UFUNCTION()
	void PopulationChanged(int16 Change);

	// --------------------------------------- Efficiency ---------------------------------------
	// When a Building Pop is not influenced by any modifiers and has exactly the pop as the default
	// max pop it will be at 100% (1.0f) efficiency. If there is less pop the efficiency will be lower.
	// When efficiency is lower, the building will work slower and vice versa

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEfficiencyChangedSig, float, EfficiencyChange);

public:
	FOnEfficiencyChangedSig OnEfficiencyChanged;

	// Building Efficiency at 1.0f will work at the default speed
	float GetEfficiency() const { return Efficiency; }

private:
	UPROPERTY(VisibleInstanceOnly)
	float Efficiency;

	void SetEfficiency(const float NewEfficiency);

	void RefreshEfficiency();

	// ------------------------------------- Predicted Production ---------------------------------------
	// How much will this building produce? Including direct production, civilian and any modifiers.
	// The main use for this is for the AI and player to know how much resource production their settlement has.

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPredictedProductionChangedSig,
	                                             float, PredictedProductionChange,
	                                             EProductionType, ProductionType);

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPredictedConsumptionChangedSig,
	                                             float, PredictedConsumptionChange,
	                                             EConsumptionType, ConsumptionType);

public:
	FOnPredictedProductionChangedSig OnPredictedProductionChanged;
	FOnPredictedConsumptionChangedSig OnPredictedConsumptionChanged;

	// returns what this building is producing, can be abstract things like "construction"
	EProductionType GetProductionType() const;

	// returns the predicted Production of GetProductionType() in Units per Second
	float GetPredictedProduction() const;

	// returns what this building is consuming
	EConsumptionType GetConsumptionType() const;

	// returns the predicted Consumption of GetConsumptionType() in Units per Second
	float GetPredictedConsumption() const;

	// --------------------------------------- Direct Production ---------------------------------------
	// Direct Production will be produced every [DirectProductionTime]/[Efficiency] seconds.
	// The Amount is always the same.
public:
	float GetDirectProductionProgress() const { return DirectProductionProgress; }

private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float DirectProductionProgress = 0.0f;

	void S_ApplyDirectProduction();

	// ---------------- Civilian Entity ----------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	ACivilian* Civilian;

	void SetCivilian(ACivilian* NewCivilian);

public:
	ACivilian* GetCivilian() const { return Civilian; }

	// --------------------- Construction phase ---------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	bool IsUnderConstruction = true;

public:
	bool GetIsUnderConstruction() const { return IsUnderConstruction; }

private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	FGameResources ResourceProgress;

public:
	FGameResources GetResourceProgress() const;
	void SetResourceProgress(const FGameResources NewResourcesProgress);

	void FinishConstruction();
};
