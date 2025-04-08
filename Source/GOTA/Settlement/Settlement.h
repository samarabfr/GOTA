// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "ConstructionResources.h"
#include "GOTA/Utility/Enums.h"
#include "GameFramework/Actor.h"
#include "Settlement.generated.h"

class UPopulation;
class AArmy;
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
	void S_RefreshBorderingUnclaimedTiles();
	
	UPROPERTY(EditDefaultsOnly)
	TArray<UBuildingSettings*> StartingBuildings;

public:
	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	TArray<ATile*> ClaimedTiles;

	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	TArray<ATile*> BorderingUnclaimedTiles;

	bool IsBorderingUnclaimedTile(const ATile* Tile) const;

	void S_RegisterTile(ATile* Tile, UBuilding* Building);
	void RegisterPopulation(UPopulation* InPopulation);

	void S_UnregisterTile(ATile* Tile, UBuilding* Building);
	void UnregisterPopulation(UPopulation* InPopulation);
	int32 GetCountOfConstructionSites() const;
	int32 GetCountOfUnprotectedBuildings() const;
	TArray<UBuilding*> GetAllBuildings() const;
	bool CanAddConstructionSite() const;
	TArray<ATile*> FindTilesWithMostNeighborBuildings() const;

	// --------------------------- Resources ---------------------------
private:
	UPROPERTY(EditDefaultsOnly)
	FConstructionResources StartingResources;
	
	UPROPERTY(EditDefaultsOnly)
	int32 ExtraAllowedConstructionSites = 2;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Settlement")
	FConstructionResources Resources;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Settlement")
	FConstructionResources Production;
	
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Settlement")
	FConstructionResources MaximumProduction;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Settlement")
	FConstructionResources Consumption;

	void S_RegisterBuildingForIncome(UBuilding* Building);
	void S_UnregisterBuildingForIncome(UBuilding* Building);

	UFUNCTION()
	void S_UpdateProduction(const float Change, float _, const EProductionType Type);

	UFUNCTION()
	void S_UpdateConsumption(const float Change, float _, EResource Type);

	UFUNCTION()
	void S_UpdateConsumptionFromPopulation(int16 Change);

public:
	FConstructionResources GetResources() const { return Resources; }
	FConstructionResources GetEffectiveProduction() const { return Production - Consumption; }
	FConstructionResources GetProduction() const { return Production; }
	FConstructionResources GetConsumption() const { return Consumption; }
	FConstructionResources GetMaximumProduction() const { return MaximumProduction; }
	FConstructionResources GetMaximumEffectiveProduction() const { return MaximumProduction - Consumption; }

	void S_AddResources(FConstructionResources Amount);
	void S_RemoveResources(FConstructionResources Amount);
	void C_AddResources(FConstructionResources Amount);
	void C_RemoveResources(FConstructionResources Amount);


	// --------------------------- Civilians ---------------------------

public:
	int32 GetCountOfBuilders() const;
	TArray<ACivilian*> GetAllCivilians() const;

	// --------------------------- Armies ---------------------------
public:
	TArray<AArmy*> GetAllArmies() const;
	
	// ----------------------- Logging -----------------------
public:
	virtual TSharedPtr<FJsonObject> Log() const;
};
