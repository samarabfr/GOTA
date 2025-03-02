// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BuildingPlacer.generated.h"

class AGS_Ingame;
class ATile;
class UBuildingSettings;
class AMouseUtils;
class UBuildingPlacerSettings;

UCLASS()
class GOTA_API ABuildingPlacer : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	// ----------------- LifeCycle -----------------

	ABuildingPlacer();

	virtual void BeginPlay() override;

public:
	void S_Init(AMouseUtils* InMouseUtils);

private:
	virtual void Tick(float DeltaSeconds) override;

	// ------------- Variables -----------------
private:
	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* PlacingPossibleMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* PlacingImpossibleMaterial = nullptr;

	UPROPERTY(ReplicatedUsing=OnRep_BuildingToPlace)
	UBuildingSettings* BuildingToPlace = nullptr;

	UPROPERTY(ReplicatedUsing=OnRep_MouseUtils)
	AMouseUtils* MouseUtils;

	TWeakObjectPtr<AGS_Ingame> GameState;

	UFUNCTION()
	void OnRep_MouseUtils();

	// ----------------- Placing -----------------

public:
	void PlaceBuilding();

	bool IsPlacing() { return BuildingToPlace != nullptr; }

private:
	UFUNCTION(Server, Reliable)
	void SRPC_PlaceBuilding(ATile* Tile, UBuildingSettings* Building);

	UFUNCTION()
	void RefreshPlaceability(ATile* NewTile);

	bool CanPlace(ATile* Tile);

	// ----------------- Start & Stop Placing -----------------

public:
	void StartPlacingBuilding(UBuildingSettings* Building);

	void StopPlacingBuilding();

private:
	UFUNCTION(Server, Reliable)
	void SRPC_StartPlacingBuilding(UBuildingSettings* Building);

	UFUNCTION(Server, Reliable)
	void SRPC_StopPlacingBuilding();

	UFUNCTION()
	void OnRep_BuildingToPlace();

	void StartShowingPlacingBuilding();

	void StopShowingPlacingBuilding();
};
