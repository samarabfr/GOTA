// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSummary.h"
#include "BuildingProject.h"
#include "SettlementBalance.h"
#include "PopulationSummary.h"
#include "SettlementImportanceRatings.h"
#include "GameFramework/Actor.h"
#include "GOTA/CoreSystems/Entity/Army.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"
#include "Settlement.generated.h"

UCLASS(Abstract, Blueprintable)
class ASettlement : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	ASettlement();

	// ---------------------------------------------------------
	// Setup
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Settlement")
	USettlementBalance* SettlementBalance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Settlement")
	ECultureLoyalty PrimaryCulture = ECultureLoyalty::MAX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Settlement")
	EAffiliation Affiliation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Settlement")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Settlement")
	UPopulationSummary* PopulationSummary;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Settlement")
	UGOTAAttributeLimited* Expansion;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttribute* Food;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttribute* Wood;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttribute* Stone;

	UPROPERTY(VisibleInstanceOnly, Instanced, BlueprintReadWrite, BlueprintSetter=SetCurrentBuildingProject, Replicated,
		Category="Settlement")
	UBuildingProject* CurrentBuildingProject;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Attribute")
	UBuildingSummary* BuildingSummary;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Settlement")
	TArray<ATile*> ClaimedTiles;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Settlement")
	FSettlementImportanceRatings ImportanceRatings;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Settlement")
	TMap<UBuildingDataAsset*, int32> ScoresDebug;

	UFUNCTION(BlueprintSetter)
	void SetCurrentBuildingProject(UBuildingProject* NewCurrentBuildingProject);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void CalculateTurn();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void OnBuildingAdded(UBuilding* Building);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void OnBuildingRemoved(UBuilding* Building);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void LostClaim(ATile* Tile);

	FPrimitiveInstanceId AddClaimMeshInstance(FTransform& Transform);
	void RemoveClaimMeshInstance(FPrimitiveInstanceId InstanceId);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AArmy> ArmyClass;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* ClaimMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* ClaimMeshRiver;

	UPROPERTY()
	UInstancedStaticMeshComponent* ISM_ClaimWalls;

	UPROPERTY()
	UInstancedStaticMeshComponent* ISM_ClaimWallsRiver;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void InitialStartingSetup(ATile* SpawnTile);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void ClaimTile(ATile* Tile);

	UFUNCTION(BlueprintCallable)
	bool ClaimRandomTile();

private:
	UPROPERTY()
	TMap<UBuildingProject*, int32> Scores;
	
	UPROPERTY()
	TSet<ATile*> BorderingUnclaimedTiles;

	UPROPERTY()
	ATileMap* TileMap;

	UPROPERTY()
	TArray<UBuildingProject*> BuildingProjectPool;

	UPROPERTY()
	TArray<UBuildingDataAsset*> PossibleBuildings;

	void GenerateBuildingIncomeAlly();

	void GenerateBuildingIncomeEnemy();

	void GenerateBaseIncome();

	void GenerateBuildingIncome();

	void FigureOutBuilding();

	void SelectNewBuildingProject();

	void FillBuildingPool();

	void CalculateImportances();

	void FigureOutSendingArmy();

	float CalculateArmySpawnChance();

	bool SpawnArmy();
};
