// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GOTA/CoreSystems/Faction/Settlement/ConstructionResources.h"
#include "GOTA/CoreSystems/Utility/Enums.h"

#include "Building.generated.h"

class AGS_Ingame;
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
	virtual void S_Init(UBuildingSettings* InSettings, ATile* InTile,
	                    ASettlement* InSettlement, AGS_Ingame* InGameState);
	void C_Init();

	virtual void S_Tick(float DeltaSeconds);
	virtual void C_Tick(const float DeltaSeconds);

	virtual void PrepareDelete();

	// ---------------------------------------- Utility ----------------------------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Building")
	UBuildingSettings* Settings;

	UPROPERTY()
	AGS_Ingame* GameState;

	UPROPERTY(Replicated)
	ATile* Tile;

public:
	UBuildingSettings* GetSettings() const { return Settings; }
	ATile* GetTile() const { return Tile; }
	AGS_Ingame* S_GetGameState() const { return GameState; }

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

	UPROPERTY(VisibleInstanceOnly, Replicated)
	float Efficiency;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	bool bIsProductionActive = true;

	void S_SetEfficiency(const float NewEfficiency);

	void S_RefreshEfficiency();

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
	void S_SetConstructionProgress(const FConstructionResources NewConstructionProgress);

	virtual void S_FinishConstruction();

	// --------------------- Production ---------------------

private:
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_ProductionPerSecond)
	float ProductionPerSecond = 0.0f;
	void S_RecalculateProductionPerSecond();

	UFUNCTION()
	void OnRep_ProductionPerSecond(float OldProductionPerSecond);

public:
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnProductionPerSecondChangedSig, float, Change, float, NewValue,
	                                               EProductionType, ProductionType);

	FOnProductionPerSecondChangedSig OnProductionPerSecondChanged;
	float GetProductionPerSecond() const;
	EProductionType GetProductionType() const;

	// --------------------- Consumption ---------------------

private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	UResourceStorage* ResourceStorage;

	UFUNCTION()
	void S_HandleStorageEmptyChanged(bool IsEmpty);

public:
	UResourceStorage* GetResourceStorage() const { return ResourceStorage; }
	EResource GetConsumptionType() const;

	// --------------------- Protection ---------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	TArray<TWeakObjectPtr<UBuilding>> Protectors;

	UPROPERTY(VisibleInstanceOnly)
	int32 MaxProtectionSearchRange = 3;

	void S_CheckForProtection();

public:
	bool IsProtected() const;
	void S_RegisterProtector(UBuilding* Protector);
	void S_UnregisterProtector(UBuilding* Protector);

	// --------------------- Civilian ---------------------

public:
	virtual ACivilian* GetCivilian() const { return nullptr; }

	// --------------------- Army ---------------------

public:
	virtual AArmy* GetArmy() const { return nullptr; }

	// --------------------- Defense building ---------------------

public:
	virtual void S_BuildingDefenseTakeDamage(int32 Damage)
	{
	}
};
