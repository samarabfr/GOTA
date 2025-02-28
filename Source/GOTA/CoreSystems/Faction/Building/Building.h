// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GOTA/CoreSystems/Faction/Settlement/ConstructionResources.h"
#include "GOTA/CoreSystems/Utility/Enums.h"

#include "Building.generated.h"

class UResourceStorage;
class UProduction;
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
	virtual void S_Init(UBuildingSettings* InSettings, ATile* InTile, ASettlement* InSettlement);
	void C_Init();

	virtual void S_Tick(float DeltaSeconds);
	virtual void C_Tick(const float DeltaSeconds);

	virtual void S_PrepareDestroy();
	void C_PrepareDestroy();

	// ---------------------------------------- Utility ----------------------------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Building")
	UBuildingSettings* Settings;

	UPROPERTY(Replicated)
	ATile* Tile;

public:
	UBuildingSettings* GetSettings() const { return Settings; }
	ATile* GetTile() const { return Tile; }

	// --------------------------------------- Settlement ---------------------------------------
private:
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Settlement)
	ASettlement* Settlement;

	UFUNCTION()
	void OnRep_Settlement();

	void S_SetSettlement(ASettlement* InSettlement);

public:
	ASettlement* GetSettlement() const { return Settlement; }

	// --------------------------------------- Population ---------------------------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Building")
	UPopulation* Population;

	UFUNCTION()
	void PopulationChanged(int16 Change);

public:
	UPopulation* GetPopulation() const { return Population; }

	// --------------------------------------- Efficiency ---------------------------------------
	// When a Building Pop is not influenced by any modifiers and has exactly the pop as the default
	// max pop it will be at 100% (1.0f) efficiency. If there is less pop the efficiency will be lower.
	// When efficiency is lower, the building will work slower and vice versa
protected:
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

	// --------------------- Construction phase ---------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	bool bIsUnderConstruction = true;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	FConstructionResources ConstructionProgress;

public:
	bool GetIsUnderConstruction() const { return bIsUnderConstruction; }

	FConstructionResources GetConstructionProgress() const;
	void SetConstructionProgress(const FConstructionResources NewConstructionProgress);

	virtual void FinishConstruction();

	// --------------------- Production ---------------------

private:
	UPROPERTY(Replicated)
	UProduction* Production;

public:
	UProduction* GetProduction();

	// --------------------- Production ---------------------

private:
	UPROPERTY(Replicated)
	UResourceStorage* ResourceStorage;
	
public:
	UResourceStorage* GetResourceStorage() const { return ResourceStorage; }

	// --------------------- Protection ---------------------

public:
	bool IsProtected() const;

	// --------------------- Army ---------------------

public:
	virtual AArmy* GetArmy() const { return nullptr; }

	// --------------------- Defense building ---------------------

public:
	virtual void S_BuildingDefenseTakeDamage(int32 Damage)
	{
	}
};
