// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GOTA/CoreSystems/Faction/Settlement/GameResources.h"
#include "GOTA/CoreSystems/Utility/Enums.h"

#include "Building.generated.h"

class AArmy;
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

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	// ---------------------------------------- Lifecycle ----------------------------------------
protected:
	UBuilding();

public:
	void ServerInit(UBuildingSettings* InSettings, ATile* InTile, ASettlement* InSettlement);
	void ClientInit();

	virtual void ServerTick(float DeltaSeconds);
	virtual void ClientTick(const float DeltaSeconds);

protected:
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
private:
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEfficiencyChangedSig, float, EfficiencyChange);

	UPROPERTY(VisibleInstanceOnly)
	float Efficiency;

	void SetEfficiency(const float NewEfficiency);

	void RefreshEfficiency();

public:
	FOnEfficiencyChangedSig OnEfficiencyChanged;

	// Building Efficiency at 1.0f will work at the default speed
	float GetEfficiency() const { return Efficiency; }


	// ------------------------------------- Predicted Production ---------------------------------------
	// How much will this building produce? Including direct production, civilian and any modifiers.
	// The main use for this is for the AI and player to know how much resource production their settlement has.
private:
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
	
	// ---------------- Army ----------------
private:

	UPROPERTY(VisibleInstanceOnly, Replicated)
	AArmy* Army;

	UPROPERTY(VisibleInstanceOnly)
	float ArmyRespawnTimer = 0.0F;

	void SetArmy(AArmy* NewArmy);

public:
	AArmy* GetArmy() const { return Army; }
	
	// --------------------- Construction phase ---------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	bool bIsUnderConstruction = true;

public:
	bool GetIsUnderConstruction() const { return bIsUnderConstruction; }

private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	FGameResources ConstructionProgress;

public:
	FGameResources GetConstructionProgress() const;
	void SetConstructionProgress(const FGameResources NewConstructionProgress);

	virtual void FinishConstruction();
};
