// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSummary.h"
#include "BuildingProject.h"
#include "BuildingProjectScore.h"
#include "SettlementSettings.h"
#include "SettlementPopulation.h"
#include "SettlementImportanceRatings.h"
#include "GameFramework/Actor.h"
#include "Settlement.generated.h"

UCLASS(Abstract, Blueprintable)
class ASettlement : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	ASettlement();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

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
	
	// -------------------Building-------------------------
public:
	UPROPERTY()
	TArray<UBuildingProject*> BuildingProjectPool;

	UPROPERTY()
	TArray<UBuildingDataAsset*> PossibleBuildings;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Settlement")
	UBuildingSummary* BuildingSummary;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Settlement")
	UBuildingProject* CurrentBuildingProject;

	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	FSettlementImportanceRatings ImportanceRatings;

	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	TArray<FBuildingProjectScore> Scores;

	void SetCurrentBuildingProject(UBuildingProject* NewCurrentBuildingProject);

	void OnBuildingAdded(UBuilding* Building, ATile* Tile);

	void OnBuildingRemoved(UBuilding* Building, ATile* Tile);

	void FigureOutBuilding();

	void SelectNewBuildingProject();

	void FillBuildingPool();

	void CalculateImportances();

	// -------------------Army??-------------------------
private:
	void FigureOutSendingArmy();

	float CalculateArmySpawnChance();

	bool SpawnArmy();
};
