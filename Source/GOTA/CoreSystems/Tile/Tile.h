// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "HexCoords.h"
#include "SpawnLayout.h"
#include "TileContent.h"
#include "TileGraphicsDataAsset.h"
#include "TileAssetWithPosition.h"
#include "TileLayout.h"
#include "BiomesDataAsset.h"
#include "EcoValues.h"
#include "Terrain.h"
#include "GameFramework/Actor.h"
#include "GOTA/CoreSystems/Entity/Entity.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "Tile.generated.h"

class ACivilian;
class UBuildingDataAsset;
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
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetAlliedEntity, Replicated, Category="Tile")
	AEntity* AlliedEntity;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetEnemyEntity, Replicated, Category="Tile")
	AEntity* EnemyEntity;

	AEntity* GetEntityByAffiliation(EAffiliation Affiliation) const;

public:
	UPROPERTY(BlueprintAssignable)
	FOnEntityChangedSignature OnEntityChanged;

	UFUNCTION(BlueprintGetter)
	AEntity* GetAlliedEntity();

	UFUNCTION(BlueprintGetter)
	AEntity* GetEnemyEntity();

	void SetAlliedEntity(AEntity* NewAlliedEntity);
	void SetEnemyEntity(AEntity* NewEnemyEntity);

	AEntity* GetEntity(EAffiliation Affiliation);
	void SetEntity(AEntity* NewEntity, EAffiliation Affiliation);

	UFUNCTION(BlueprintCallable, Category="Tile")
	bool IsWalkable(EAffiliation Affiliation) const;

	bool AcceptsEntity(EEntityType EntityType) const;

	// ------------------------Civilians---------------------------
private:
	UPROPERTY()
	TArray<ACivilian*> Civilians;

public:
	bool AcceptsCivilian() const;
	void AddCivilian(ACivilian* Civilian);
	void RemoveCivilian(ACivilian* Civilian);

	// -------------------Claimant and claiming-------------------

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintSetter=SetClaimant, BlueprintGetter=GetClaimant,
		ReplicatedUsing=OnRep_Claimant, Category="Tile")
	ASettlement* Claimant;

	TMap<uint8, FPrimitiveInstanceId> ClaimWallsInstanceIds;

public:
	UFUNCTION(BlueprintGetter)
	ASettlement* GetClaimant();

	UFUNCTION(BlueprintSetter)
	void SetClaimant(ASettlement* NewClaimant);

private:
	UFUNCTION()
	void OnRep_Claimant(ASettlement* NewClaimant);

public:
	void UpdateClaimWallsWithNeighbors();

	void UpdateClaimWalls();

	UFUNCTION(BlueprintCallable, Category="Tile")
	bool IsClaimable() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Tile")
	bool TryClaim(ASettlement* PotentialClaimant);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Tile")
	void Unclaim();

	// -----------------------Building---------------------
	
	bool CanBuild();
	bool TryBuild(UBuildingDataAsset* BuildingDataAsset, ASettlement* Builder);
	void Unbuild();
	
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Building, Category="Tile")
	UBuilding* Building;

	UFUNCTION()
	void OnRep_Building();

	void BuildingChanged();
	
	FOnTileChangedSignature OnBuildingChanged;

	// -------------------Ticking-------------------------
private:
	double LastTick = -1.0;
	
public:
	void GOTATick();

	UFUNCTION()
	void PopSizeChanged(const int16 Change);
	
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UEcoValues* EcoValues;

	UFUNCTION()
	void TreesChanged(const int32 Change);

	UFUNCTION()
	void WildlifeChanged(const int32 Change);

	UFUNCTION()
	void ForageChanged(const int32 Change);

	// -----------------------Graphics--------------------------
public:
	UPROPERTY(EditDefaultsOnly, Category="Tile")
	UTileGraphicsDataAsset* DA_TileGraphics;

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
