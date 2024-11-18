// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "GameResources.h"
#include "SettlementSettings.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	TArray<ATile*> ClaimedTiles;

public:
	UPROPERTY()
	TArray<ATile*> BorderingUnclaimedTiles;

protected:
	void RefreshBorderingUnclaimedTiles();

public:
	bool IsBorderingUnclaimedTile(const ATile* Tile) const;

	// --------------------------- Building ---------------------------
public:
	void OnBuildingAdded(UBuilding* Building, ATile* Tile);

	void OnBuildingRemoved(UBuilding* Building, ATile* Tile);

	// --------------------------- Resources ---------------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Settlement")
	FGameResources Resources;
	
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Settlement")
	FGameResources PredictedIncome;

public:
	FGameResources GetResources() const { return Resources; }
	void S_AddResources(FGameResources Amount);
	void S_RemoveResources(FGameResources Amount);
};
