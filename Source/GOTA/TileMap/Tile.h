// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "HexCoords.h"
#include "SpawnPointLayout.h"
#include "TileContent.h"
#include "TileGraphicsDataAsset.h"
#include "TileLayout.h"
#include "GOTA/Faction/Entity.h"
#include "GameFramework/Actor.h"
#include "GOTA/EcoSystemDataAsset.h"
#include "GOTA/Faction/GOTAAttributeLimited.h"
#include "Tile.generated.h"

class UBuilding;
class ASettlement;

UCLASS()
class GOTA_API ATile : public AActor
{
	//Unreal Engine Mystery Code
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Constructor
	ATile();

	virtual void BeginPlay() override;
	
public:
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	FGameplayTagContainer GameplayTags;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	FHexCoords HexCoords;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileMap")
	void Init();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Tile")
	void Claim(const ASettlement* PotentialClaimant, bool& Success);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Tile")
	void Unclaim();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Tile")
	bool TryBuild(UBuildingDataAsset* BuildingDataAsset);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Tile")
	void Unbuild();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Tile")
	void OnEnteringActiveRangeOfGuardian();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Tile")
	void OnLeavingActiveRangeOfGuardian();

	void CalculateTurn();

	void CalculateTreeGrowthChange();

	UFUNCTION()
	void CalculateTreeGrowthChangeWithNeighbors(int32 Change);

	void CalculateForageChange();

	UFUNCTION()
	void CalculateForageChangeWithNeighbors(int32 Change);

	void CalculateWildlifeGrowthChange();

	UFUNCTION()
	void CalculateWildlifeGrowthChangeWithNeighbors(int32 Change);

	void CalculatePopulationGrowthChange();

	UFUNCTION()
	void CalculatePopulationGrowthChangeWithNeighbors();

	UFUNCTION(BlueprintCallable, Category="Tile")
	bool IsWalkable(EAffiliation Affiliation) const;

	UFUNCTION(BlueprintCallable, Category="Tile")
	bool IsClaimable() const;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	UEcoSystemDataAsset* BalanceData;

public:
	UPROPERTY(BlueprintReadWrite, Replicated, Category="Tile")
	UBuilding* Building;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Tile")
	void AddBuildingToReplication();

private:
	UPROPERTY(BlueprintSetter=SetClaimant, BlueprintGetter=GetClaimant, ReplicatedUsing=OnRep_Claimant, Category="Tile")
	ASettlement* Claimant;

	UFUNCTION()
	void OnRep_Claimant(ASettlement* NewClaimant);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Tile")
	void ClaimantChanged();

public:
	UFUNCTION(BlueprintGetter)
	ASettlement* GetClaimant();

	UFUNCTION(BlueprintSetter)
	void SetClaimant(ASettlement* NewClaimant);

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UGOTAAttributeLimited* Trees;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UGOTAAttribute* TreeGrowth;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UGOTAAttribute* TreeGrowthChange;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UGOTAAttributeLimited* Forage;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UGOTAAttribute* ForageChange;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UGOTAAttributeLimited* Wildlife;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UGOTAAttribute* WildlifeGrowth;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UGOTAAttribute* WildlifeGrowthChange;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	TArray<ATile*> Neighbors; // 0 = North, 1 = NorthEast, 2 = SouthEast, 3 = South, 4 = SouthWest, 5 = NorthWest

	static bool bFreezeGrowthChanges;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	AEntity* AlliedTileEntity;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	AEntity* EnemyTileEntity;

	// River

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	bool bIsRiver;
	
	void SetIsRiver(bool IsRiver);


	UPROPERTY(BlueprintReadwrite, BlueprintSetter=SetBiome, ReplicatedUsing=OnRep_Biome, Category="Tile")
	EBiome Biome = EBiome::Gras;

	UFUNCTION(BlueprintSetter)
	void SetBiome(EBiome NewBiome);

	UFUNCTION()
	void OnRep_Biome();

	// ---------------------------------------------------------
	// TileContent relevant
private:
	EBiome MaterialBiome;
	
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="TileContent")
	void UpdateHexagonMaterial();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	UTileGraphicsDataAsset* DA_TileGraphics;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	UDataTable* TileLayouts;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* SM_Hexagon;
	
	UPROPERTY(BlueprintReadOnly, Category="Tile")
	FTileLayout TileLayout;
	
	// Check if anything needs to be changed and do that
	void RefreshTileLayout();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_SpawnPointLayout, Category="Tile")
	FSpawnPointLayout SpawnPointLayout;

	UFUNCTION()
	void OnRep_SpawnPointLayout();

	UFUNCTION(BlueprintImplementableEvent, Category="Tile")
	void OnSpawnPointLayoutChanged();

	UFUNCTION(BlueprintImplementableEvent, Category="Tile")
	void SpawnTileContent();

	UPROPERTY(BlueprintReadWrite)
	ATileContent* TileContent;

private:
	void RecalculateTileLayout();

	FTileLayout* FindNewValidTileLayout() const;

	bool IsValidTileLayout(const FTileLayout* Layout) const;

protected:
	// Returns -1 when none found, returns rotation ID (0-5) if one is found
	UFUNCTION(BlueprintCallable, Category="Tile")
	int32 FindRiverConnectionRotation(const TArray<bool> Connections) const;
};
