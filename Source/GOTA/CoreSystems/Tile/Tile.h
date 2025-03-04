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
#include "GOTA/CoreSystems/Faction/Building/Building.h"
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
	void S_Init();
	
	void Delete();

	// ------------------------Tilemap---------------------------
private:
	UPROPERTY()
	AGS_Ingame* GameState;

public:
	UPROPERTY(VisibleInstanceOnly, Replicated, BlueprintReadOnly)
	FHexCoords HexCoords;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Tile")
	TArray<ATile*> Neighbors;

	TArray<ATile*> GetNeighbors() const { return Neighbors; }

	TArray<ATile*> GetPathTo(ATile* Target);
	int32 GetPathTileDistanceTo(ATile* Target);
	int32 GetTileDistanceTo(const ATile* Target) const;
	
	// ------------------------Gameplay Tags---------------------------

public:
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_GameplayTags, Category="Tile")
	FGameplayTagContainer GameplayTags;

	UFUNCTION()
	void OnRep_GameplayTags();

	UPROPERTY()
	FOnChangedSignature OnGameplayTagsChanged;

	// ------------------------Entity---------------------------
public:
	bool AcceptsEntity(const EEntityType EntityType) const;
	void AddEntity(AEntity* Entity, const EEntityType EntityType, FVector& NewLocation);
	void RemoveEntity(AEntity* Entity, const EEntityType EntityType);

	// ------------------------Army---------------------------
private:
	UPROPERTY(VisibleInstanceOnly)
	TWeakObjectPtr<AArmy> Army;

public:
	AArmy* GetArmy() const;
	bool AcceptsArmy() const;
	void SetArmy(AArmy* NewArmy, FVector& NewLocation);
	void RemoveArmy();

	// ------------------------Civilians---------------------------
private:
	UPROPERTY(VisibleInstanceOnly)
	TArray<ACivilian*> Civilians;

public:
	TArray<ACivilian*> GetCivilians() const { return Civilians; }
	bool AcceptsCivilian() const;
	void AddCivilian(ACivilian* Civilian, FVector& NewLocation);
	void RemoveCivilian(ACivilian* Civilian);

	// ----------------------- Building and Claiming ---------------------

private:
	TMap<uint8, FPrimitiveInstanceId> ClaimWallsInstanceIds;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Building, Category="Tile")
	UBuilding* Building;

	UFUNCTION()
	void OnRep_Building(UBuilding* OldBuilding);

	void BuildingChanged();

public:
	UBuilding* GetBuilding() const { return Building; }

	FOnTileChangedSignature OnBuildingChanged;

	bool CanBuild(UBuildingSettings* BuildingDataAsset, ASettlement* Builder);

	bool S_TryBuild(UBuildingSettings* BuildingDataAsset, ASettlement* Builder);

	bool S_TryForceBuild(UBuildingSettings* BuildingDataAsset, ASettlement* Builder);

	void S_Unbuild();

	void OnBuildingFinishedConstruction();

	ASettlement* GetClaimant() const { return Building == nullptr ? nullptr : Building->GetSettlement(); }

	bool IsClaimed() { return Building != nullptr; }

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

	/* ----------------------- Terrain ---------------------------
	 * Terrain is only Set in TerrainInit (called by WorldGen) and nowhere else.
	 * No Value of Terrain is allowed to change after TerrainInit was called.
	 * IDK how we could enforce it on compiler level tho
	 */
	UPROPERTY(EditDefaultsOnly, Category="Tile")
	UBiomesDataAsset* DA_Biomes;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=C_TerrainInit, Category="Tile")
	FTerrain Terrain;

	UFUNCTION()
	void C_TerrainInit();

public:
	void S_TerrainInit(const FTerrain& Terrain_);
	FTerrain GetTerrain() const { return Terrain; }

private:
	void UpdateHexagonMaterial();
	void SpawnOceanLineMeshes();

	// ------------------TileContent--------------------
private:
	UPROPERTY()
	UTileContent* TileContent;

	void InitTileContent();

	UPROPERTY(ReplicatedUsing=InitTileRotation)
	float TileRotation = 0.0;

	UFUNCTION()
	void InitTileRotation();
	void S_InitTileRotation();

	// ---------------------TileLayout--------------------

	FTileLayout* TileLayout;

	void InitTileLayout();
	
	// ---------------------SpawnLayout--------------------
	
	FSpawnLayoutStruct* SpawnLayoutStruct;
	
public:
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_SpawnPointLayout, Category="Tile")
	FSpawnLayout SpawnLayout;

private:
	void SetSpawnLayout(const FSpawnLayout& SpawnLayout_);

	UFUNCTION()
	void OnRep_SpawnPointLayout();

	void ValidateSpawnLayout();

	void ApplySpawnChances(TArray<FSpawnPoint>& SpawnPoints);

	FSpawnLayoutStruct* FindSpawnLayoutDataAsset();
};
