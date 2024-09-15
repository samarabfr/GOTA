// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSummary.h"
#include "BuildingProject.h"
#include "BuildingProjectScore.h"
#include "SettlementBalance.h"
#include "SettlementPopulation.h"
#include "SettlementImportanceRatings.h"
#include "GameFramework/Actor.h"
#include "GOTA/CoreSystems/Entity/Army.h"
#include "GOTA/CoreSystems/Faction/Attribute/GOTAAttributeLimited.h"
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

	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Settlement")
	USettlementBalance* SettlementBalance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Settlement")
	EAffiliation Affiliation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Settlement")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Settlement")
	USettlementPopulation* PopulationSummary;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Attribute")
	FGameResources Resources;

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
	TArray<FBuildingProjectScore> Scores;

	UFUNCTION(BlueprintSetter)
	void SetCurrentBuildingProject(UBuildingProject* NewCurrentBuildingProject);

	void GenerateIncome(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void OnBuildingAdded(UBuilding* Building);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void OnBuildingRemoved(UBuilding* Building);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void LostClaim(ATile* Tile);

	FPrimitiveInstanceId AddClaimMeshInstance(FTransform& Transform);
	void RemoveClaimMeshInstance(FPrimitiveInstanceId InstanceId);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void InitialStartingSetup(ATile* SpawnTile);

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

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void ClaimTile(ATile* Tile);

	UFUNCTION(BlueprintCallable)
	bool ClaimRandomTile();

private:	
	UPROPERTY()
	TSet<ATile*> BorderingUnclaimedTiles;

	UPROPERTY()
	ATileMap* TileMap;

	UPROPERTY()
	TArray<UBuildingProject*> BuildingProjectPool;

	UPROPERTY()
	TArray<UBuildingDataAsset*> PossibleBuildings;

	void FigureOutBuilding();

	void SelectNewBuildingProject();

	void FillBuildingPool();

	void CalculateImportances();

	void FigureOutSendingArmy();

	float CalculateArmySpawnChance();

	bool SpawnArmy();
};
