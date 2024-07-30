// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "HexCoords.h"
#include "SpawnPointLayout.h"
#include "TileContent.h"
#include "TileGraphicsDataAsset.h"
#include "TileAssetWithPosition.h"
#include "TileLayout.h"
#include "GOTA/Faction/Entity.h"
#include "BiomesDataAsset.h"
#include "GameFramework/Actor.h"
#include "GOTA/EcoSystemDataAsset.h"
#include "GOTA/Faction/GOTAAttributeLimited.h"
#include "Tile.generated.h"

class UBuilding;
class ASettlement;

UCLASS()
class GOTA_API ATile : public AActor
{
	GENERATED_BODY()

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangedSignature);

	// ---------------------------------------------------------
	// Initialisation and core variables

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	ATile();

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileMap")
	void Init();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_GameplayTags, Category="Tile")
	FGameplayTagContainer GameplayTags;

	UFUNCTION()
	void OnRep_GameplayTags();

	UPROPERTY()
	FOnChangedSignature OnGameplayTagsChanged;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FHexCoords HexCoords;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	TArray<ATile*> Neighbors; // 0 = North, 1 = NorthEast, 2 = SouthEast, 3 = South, 4 = SouthWest, 5 = NorthWest

	UPROPERTY(BlueprintReadwrite, BlueprintSetter=SetIsRiver, Replicated, Category="Tile")
	bool bIsRiver;

	UFUNCTION(BlueprintSetter)
	void SetIsRiver(bool IsRiver);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	UBiomesDataAsset* DA_Biomes;

	UPROPERTY(BlueprintReadwrite, BlueprintSetter=SetBiome, Replicated, Category="Tile")
	EBiome Biome = EBiome::Gras;

	UFUNCTION(BlueprintSetter)
	void SetBiome(EBiome NewBiome);

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	AEntity* AlliedTileEntity;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	AEntity* EnemyTileEntity;

	UFUNCTION(BlueprintCallable, Category="Tile")
	bool IsWalkable(EAffiliation Affiliation) const;

	// ---------------------------------------------------------
	// Claimant and claiming

private:
	UPROPERTY(BlueprintSetter=SetClaimant, BlueprintGetter=GetClaimant, ReplicatedUsing=OnRep_Claimant, Category="Tile")
	ASettlement* Claimant;

	TMap<uint8, FPrimitiveInstanceId> ClaimFlagInstanceIds;
public:
	UFUNCTION(BlueprintGetter)
	ASettlement* GetClaimant();

	UFUNCTION(BlueprintSetter)
	void SetClaimant(ASettlement* NewClaimant);

private:
	UFUNCTION()
	void OnRep_Claimant(ASettlement* NewClaimant);

public:
	void UpdateClaimFlagsWithNeighbors();
	
	void UpdateClaimFlags();
	
	UFUNCTION(BlueprintCallable, Category="Tile")
	bool IsClaimable() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Tile")
	bool TryClaim(ASettlement* PotentialClaimant);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Tile")
	void Unclaim();

	// ---------------------------------------------------------
	// Building

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_Building, Category="Tile")
	UBuilding* Building;

	UFUNCTION()
	void OnRep_Building();

	UPROPERTY(BlueprintAssignable)
	FOnChangedSignature OnBuildingChanged;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Tile")
	bool TryBuild(UBuildingDataAsset* BuildingDataAsset);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Tile")
	void Unbuild();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Tile")
	void AddBuildingToReplication();

	// ---------------------------------------------------------
	// Weird solution for the Guardian is in X range for animation performance

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Tile")
	void OnEnteringActiveRangeOfGuardian();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Tile")
	void OnLeavingActiveRangeOfGuardian();

	// ---------------------------------------------------------
	// Ecosystem and calculate Turn 

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

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	UEcoSystemDataAsset* BalanceData;

public:
	static bool bFreezeGrowthChanges;

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

	// ---------------------------------------------------------
	// TileLayout & TileContent and graphics relevant

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Tile Graphics")
	UStaticMeshComponent* SM_Hexagon;

	UPROPERTY(BlueprintReadWrite, Category="Tile Graphics")
	ATileContent* TileContent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile Graphics")
	UTileGraphicsDataAsset* DA_TileGraphics;

	UPROPERTY(BlueprintReadOnly, Category="Tile Graphics")
	FTileLayout TileLayout;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_SpawnPointLayout, Category="Tile Graphics")
	FSpawnPointLayout SpawnPointLayout;

	UFUNCTION()
	void OnRep_SpawnPointLayout();

	UPROPERTY()
	FOnChangedSignature OnSpawnPointLayoutChanged;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_TileContentRotation)
	float TileContentRotation = 0.0;

	UFUNCTION()
	void OnRep_TileContentRotation();

private:
	EBiome MaterialBiome = EBiome::Gras;

	// Check if anything needs to be changed and do that
	void RefreshTileLayout();

	void RecalculateTileLayout();

	void ApplySpawnChances(TArray<FSpawnPoint>& SpawnPoints);

	FTileLayout* FindNewValidTileLayout() const;

	bool IsValidTileLayout(const FTileLayout* Layout) const;

	// Returns -1 when none found, returns rotation ID (0-5) if one is found
	int32 FindRiverConnectionRotation(const TArray<bool> Connections) const;

	void UpdateBuildingAssets();

public:
	UFUNCTION(BlueprintImplementableEvent, Category="Tile Graphics")
	void SpawnTileContent();

	UFUNCTION(BlueprintImplementableEvent, Category="Tile Graphics")
	void UpdateHexagonMaterial();
};
