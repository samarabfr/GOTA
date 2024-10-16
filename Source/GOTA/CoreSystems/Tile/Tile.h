// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "HexCoords.h"
#include "SpawnLayout.h"
#include "TileContent.h"
#include "TileSettings.h"
#include "TileLayout.h"
#include "BiomesDataAsset.h"
#include "EcoValues.h"
#include "Terrain.h"
#include "GameFramework/Actor.h"
#include "GOTA/CoreSystems/Entity/Entity.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "Tile.generated.h"

class AArmy;
class ACivilian;
class UBuildingSettings;
class UBuilding;
class ASettlement;

UCLASS()
class GOTA_API ATile : public AActor
{
	GENERATED_BODY()

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangedSignature);

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileChangedSignature, ATile*, Tile);

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEntityChangedSignature, ATile*, Tile, AEntity*, OldEntity);

	// ---------------------------------------------------------
	// Initialisation and core variables

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	ATile();

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileMap")
	void ServerInit();

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_GameplayTags, Category="Tile")
	FGameplayTagContainer GameplayTags;

	UFUNCTION()
	void OnRep_GameplayTags();

	UPROPERTY()
	FOnChangedSignature OnGameplayTagsChanged;

	UPROPERTY(VisibleInstanceOnly, Replicated, BlueprintReadOnly)
	FHexCoords HexCoords;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Tile")
	TArray<ATile*> Neighbors;

private:
	UPROPERTY()
	AGS_Ingame* GameState;

	// ------------------------Entity---------------------------
public:
	bool AcceptsEntity(const EEntityType EntityType) const;
	
	// ------------------------Army---------------------------
private:
	UPROPERTY(VisibleInstanceOnly)
	AArmy* Army;

public:
	AArmy* GetArmy() const;
	bool AcceptsArmy() const;
	void SetArmy(AArmy* NewArmy);
	void RemoveArmy();

	// ------------------------Civilians---------------------------
private:
	UPROPERTY(VisibleInstanceOnly)
	TArray<ACivilian*> Civilians;

public:
	bool AcceptsCivilian() const;
	void AddCivilian(ACivilian* Civilian, FVector& NewLocation);
	void RemoveCivilian(const ACivilian* Civilian);

	// ----------------------- Building and Claiming ---------------------

private:
	TMap<uint8, FPrimitiveInstanceId> ClaimWallsInstanceIds;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Building, Category="Tile")
	UBuilding* Building;
	
	UFUNCTION()
	void OnRep_Building();

	void BuildingChanged();

public:
	UBuilding* GetBuilding() const { return Building; }
	
	FOnTileChangedSignature OnBuildingChanged;

	bool CanBuild();

	bool TryBuild(UBuildingSettings* BuildingDataAsset, ASettlement* Builder);

	void Unbuild();

	void OnBuildingFinishedConstruction();

	ASettlement* GetClaimant() const;

	bool IsClaimed() { return Building != nullptr; }

private:
	void UpdateClaimWallsWithNeighbors();

	void UpdateClaimWalls();

	// ------------------- Ticking -------------------------
private:
	double LastTick = -1.0;

public:
	void GOTATick();

	void SetupPopSizeChanging();

	UFUNCTION()
	void PopSizeChanged(const int16 Change);

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UEcoValues* EcoValues;
	
	void SetupEcoValuesChanging();
	
	UFUNCTION()
	void TreesChanged(const int32 Change);

	UFUNCTION()
	void WildlifeChanged(const int32 Change);

	UFUNCTION()
	void ForageChanged(const int32 Change);

	// -----------------------Graphics--------------------------
public:
	UPROPERTY(EditDefaultsOnly, Category="Tile")
	UTileSettings* Settings;

private:
	UPROPERTY(EditDefaultsOnly, Category="Tile")
	UStaticMeshComponent* SM_Hexagon;

	UPROPERTY(ReplicatedUsing=InitHexagonMesh)
	UStaticMesh* HexagonMesh;

	UFUNCTION()
	void InitHexagonMesh();

	/* -----------------------Terrain---------------------------
	 * Terrain is only Set in TerrainInit (called by WorldGen) and nowhere else.
	 * No Value of Terrain is allowed to change after TerrainInit was called.
	 * IDK how we could enforce it on compiler level tho
	 */
	UPROPERTY(EditDefaultsOnly, Category="Tile")
	UBiomesDataAsset* DA_Biomes;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=TerrainClientInit, Category="Tile")
	FTerrain Terrain;

	UFUNCTION()
	void TerrainClientInit();

public:
	void TerrainServerInit(const FTerrain& Terrain_);

private:
	void UpdateHexagonMaterial();

	// ------------------TileContent--------------------
private:
	UPROPERTY()
	UTileContent* TileContent;

	void InitTileContent();

	UPROPERTY(ReplicatedUsing=ClientInitTileRotation)
	float TileRotation = 0.0;

	UFUNCTION()
	void ClientInitTileRotation();
	void ServerInitTileRotation();

	// ---------------------TileLayout--------------------

	FTileLayout* TileLayout;

	void InitTileLayout();

	FTileLayout* FindTileLayout();

	bool IsValidTileLayout(const FTileLayout* Layout) const;

	// Returns -1 when none found, returns rotation ID (0-5) if one is found
	int32 FindAValidRiverConnectionRotation(const TArray<bool> Connections) const;

	// ---------------------SpawnLayout--------------------

	UPROPERTY(VisibleInstanceOnly, Category="Tile")
	USpawnLayoutDataAsset* SpawnLayoutDataAsset;

public:
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_SpawnPointLayout, Category="Tile")
	FSpawnLayout SpawnLayout;

private:
	void SetSpawnLayout(const FSpawnLayout& SpawnLayout_);

	UFUNCTION()
	void OnRep_SpawnPointLayout();

	void ValidateSpawnLayout();

	void ApplySpawnChances(TArray<FSpawnPoint>& SpawnPoints);

	USpawnLayoutDataAsset* FindSpawnLayoutDataAsset();
};
