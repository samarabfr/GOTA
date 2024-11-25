// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementSettings.h"
#include "SettlementPopulation.h"
#include "GameFramework/Actor.h"
#include "Settlement.generated.h"

class ACivilian;
class UBuilding;
class ATile;

UCLASS(Abstract, Blueprintable)
class ASettlement : public AActor
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ------------------- LifeCycle -------------------

	ASettlement();

private:
	virtual void BeginPlay() override;

public:
	virtual void S_Init(ATile* SpawnTile, UPopulationSettings* InPopulationSettings);

	void EnableTick();

protected:
	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY()
	USettlementSettings* Settings;

private:
	UPROPERTY()
	UPopulationSettings* PopulationSettings;

public:
	UPopulationSettings* GetPopulationSettings() { return PopulationSettings; }
	
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Settlement")
	EAffiliation Affiliation;

	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	USettlementPopulation* Population;

	// -------------------Claims-------------------------

	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	TArray<ATile*> ClaimedTiles;

public:
	UPROPERTY()
	TArray<ATile*> BorderingUnclaimedTiles;

protected:
	void RefreshBorderingUnclaimedTiles();

public:
	bool IsBorderingUnclaimedTile(const ATile* Tile) const;

	// -------------------Building-------------------------
public:
	void OnBuildingAdded(UBuilding* Building, ATile* Tile);

	void OnBuildingRemoved(UBuilding* Building, ATile* Tile);

	// -------------------Resources-------------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Settlement")
	FGameResources Resources;
	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	FGameResources LastMinuteIncome;
	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	FGameResources LastMinuteConsumption;

	struct FIncomeEvent
	{
		FGameResources Amount;
		float Timestamp;

		FIncomeEvent(): Amount(), Timestamp()
		{
		};

		FIncomeEvent(const FGameResources InAmount, const float InTimestamp)
			: Amount(InAmount), Timestamp(InTimestamp)
		{
		}
	};

	TQueue<FIncomeEvent> IncomeEvents;
	TQueue<FIncomeEvent> ConsumptionEvents;
	void UpdateLastMinuteResources();

public:
	FGameResources GetResources() const { return Resources; }
	void S_AddResources(FGameResources Amount, bool CountTowardsLastMinuteIncome = false);
	void S_RemoveResources(FGameResources Amount, bool CountTowardsLastMinuteConsumption = false);
};
