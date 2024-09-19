// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSummary.h"
#include "SettlementSettings.h"
#include "SettlementPopulation.h"
#include "GameFramework/Actor.h"
#include "Settlement.generated.h"

UCLASS(Abstract, Blueprintable)
class ASettlement : public AActor
{
	GENERATED_BODY()
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	ASettlement();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void EnableTick();

public:
	UPROPERTY()
	USettlementSettings* SettlementSettings;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Settlement")
	EAffiliation Affiliation;

	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	USettlementPopulation* PopulationSummary;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Settlement")
	FGameResources Resources;

	void StartingSetup(ATile* SpawnTile);

	void GenerateIncome(float DeltaSeconds);

	// -------------------Claims-------------------------

	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	TArray<ATile*> ClaimedTiles;

protected:
	UPROPERTY()
	TArray<ATile*> BorderingUnclaimedTiles;

	void RefreshBorderingUnclaimedTiles();

public:
	bool IsBorderingUnclaimedTile(const ATile* Tile) const;
	
	// -------------------Building-------------------------
public:
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Settlement")
	UBuildingSummary* BuildingSummary;

	void OnBuildingAdded(UBuilding* Building, ATile* Tile);

	void OnBuildingRemoved(UBuilding* Building, ATile* Tile);
};
