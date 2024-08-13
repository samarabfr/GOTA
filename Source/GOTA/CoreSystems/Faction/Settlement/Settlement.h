// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSummary.h"
#include "BuildingProject.h"
#include "SettlementBalance.h"
#include "PopulationSummary.h"
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

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Settlement")
	TSubclassOf<AArmy> ArmyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Settlement")
	ECultureLoyalty PrimaryCulture = ECultureLoyalty::MAX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Settlement")
	EAffiliation Affiliation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Settlement")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Attribute")
	UPopulationSummary* PopulationSummary;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void Init();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void InitialStartingSetup(ATile* SpawnTile);

	// ---------------------------------------------------------
	// Claiming

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* ClaimMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* ClaimMeshRiver;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* ClaimMaterial;

	UPROPERTY(BlueprintReadWrite, Category="Settlement")
	TSet<ATile*> BorderingUnclaimedTiles;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Settlement")
	TArray<ATile*> ClaimedTiles;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_ClaimColor, Category="Settlement")
	FLinearColor ClaimColor;

	UFUNCTION()
	void OnRep_ClaimColor();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttributeLimited* Expansion;

	UPROPERTY()
	UInstancedStaticMeshComponent* ISM_ClaimWalls;

	UPROPERTY()
	UInstancedStaticMeshComponent* ISM_ClaimWallsRiver;

	FPrimitiveInstanceId AddClaimMeshInstance(FTransform& Transform);
	void RemoveClaimMeshInstance(FPrimitiveInstanceId InstanceId);

	UFUNCTION(BlueprintCallable)
	bool ClaimRandomTile();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void LostClaim(ATile* Tile);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void ClaimTile(ATile* Tile);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void RefreshBorderingTiles();

	// ---------------------------------------------------------
	// Turn Calculation

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void CalculateTurn();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void GenerateBaseIncome();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void GenerateBuildingIncome();

	void GenerateBuildingIncomeAlly();

	void GenerateBuildingIncomeEnemy();	

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void FigureOutBuilding();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void FigureOutSendingArmy();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	bool SpawnArmy();

	// ---------------------------------------------------------
	// Resources and Building

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttribute* Food;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttribute* Wood;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttribute* Stone;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Settlement")
	TArray<UBuildingDataAsset*> PossibleBuildings;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Attribute")
	UBuildingSummary* BuildingSummary;

	UPROPERTY(VisibleInstanceOnly, Instanced, BlueprintReadWrite, BlueprintSetter=SetCurrentBuildingProject, Replicated,
		Category="Settlement")
	UBuildingProject* CurrentBuildingProject;

	UFUNCTION(BlueprintSetter)
	void SetCurrentBuildingProject(UBuildingProject* NewCurrentBuildingProject);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void OnBuildingAdded(UBuilding* Building);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void OnBuildingRemoved(UBuilding* Building);

private:
	ATileMap* TileMap;

	float CalculateArmySpawnChance();
};
