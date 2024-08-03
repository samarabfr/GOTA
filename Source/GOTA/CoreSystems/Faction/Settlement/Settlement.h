// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingProductionSummary.h"
#include "BuildingProject.h"
#include "SettlementBalance.h"
#include "PopulationSummary.h"
#include "GameFramework/Actor.h"
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
	UInstancedStaticMeshComponent* ISM_ClaimFlags;

	FPrimitiveInstanceId AddClaimMeshInstance(FTransform& Transform);
	void RemoveClaimMeshInstance(FPrimitiveInstanceId InstanceId);

	UFUNCTION(BlueprintCallable)
	bool ClaimRandomTile();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void LostClaim(const ATile* Tile);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void ClaimTile(const ATile* Tile);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void RefreshBorderingTiles();
	
	// ---------------------------------------------------------
	// Turn Calculation

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void CalculateTurn();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void GenerateBaseIncome();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void GenerateBuildingIncome();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void FigureOutBuilding();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void FigureOutSendingArmy();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	bool SpawnArmy();

	// ---------------------------------------------------------
	// Resources and Building
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttribute* Food;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttribute* Wood;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttribute* Stone;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Settlement")
	TArray<UBuildingDataAsset*> PossibleBuildings;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category="Attribute")
	UBuildingProductionSummary* ProductionSummary;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, BlueprintSetter=SetCurrentBuildingProject, Replicated,
	Category="Settlement")
	UBuildingProject* CurrentBuildingProject;
	
	UFUNCTION(BlueprintSetter)
	void SetCurrentBuildingProject(UBuildingProject* NewCurrentBuildingProject);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void OnBuildingAdded(UBuilding* Building);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void OnBuildingRemoved(UBuilding* Building);
};
