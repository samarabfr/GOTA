// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building.h"
#include "BuildingProductionSummary.h"
#include "BuildingProject.h"
#include "SettlementBalance.h"
#include "PopulationSummary.h"
#include "GOTA/TileMap/Tile.h"
#include "GameFramework/Actor.h"
#include "Settlement.generated.h"

UCLASS(Abstract, Blueprintable)
class ASettlement : public AActor
{
	//Unreal Engine Mystery Code
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Constructor
	ASettlement();

	virtual void BeginPlay() override;

	//====================================================================
	//--------------------Delegates
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnyAttributeChangedSignature);

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAnyAttributeChangedSignature OnAnyAttributeChanged;

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAnyAttributeChangedSignature OnCurrentBuildingProjectChanged;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

	//====================================================================
	//--------------------Overrideable Functions
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void RefreshBorderingTiles();

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void Init();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void CalculateTurn();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void LostClaim(const ATile* Tile);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void ClaimTile(const ATile* Tile);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void GenerateBaseIncome();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void GenerateBuildingIncome();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void FigureOutBuilding();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void FigureOutSendingArmy();

	//====================================================================
	//-------------------- Functions
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void OnBuildingAdded(UBuilding* Building);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	void OnBuildingRemoved(UBuilding* Building);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Settlement")
	bool SpawnArmy();
	
	//====================================================================
	//--------------------Variables
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
public:
	UPROPERTY(BlueprintReadWrite, Category="Settlement")
	TSet<ATile*> BorderingUnclaimedTiles;

	UPROPERTY(BlueprintReadWrite, Category="Settlement")
	TArray<ATile*> ClaimedTiles;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="Settlement")
	FLinearColor ClaimColor;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Settlement")
	TArray<UBuildingDataAsset*> PossibleBuildings;

	UPROPERTY(BlueprintReadWrite, BlueprintSetter=SetCurrentBuildingProject, Replicated, Category="Settlement")
	UBuildingProject* CurrentBuildingProject;

	UFUNCTION(BlueprintSetter)
	void SetCurrentBuildingProject(UBuildingProject* NewCurrentBuildingProject);

	UPROPERTY(BlueprintReadWrite, Replicated, Category="Settlement")
	ECultureLoyalty PrimaryCulture = ECultureLoyalty::MAX;
	
	UPROPERTY(BlueprintReadWrite, Replicated, Category="Settlement")
	EAffiliation Affiliation;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Settlement")
	USettlementBalance* SettlementBalance;
	
	//====================================================================
	//--------------------Attributes
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttribute* Food;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttribute* Wood;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttribute* Stone;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Attribute")
	UPopulationSummary* PopulationSummary;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Attribute")
	UBuildingProductionSummary* ProductionSummary;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttributeLimited* Expansion;
};