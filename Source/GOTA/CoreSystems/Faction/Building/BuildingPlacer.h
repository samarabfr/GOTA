// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BuildingPlacer.generated.h"

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

	void C_Init();

private:
	virtual void Tick(float DeltaSeconds) override;

	// ------------- Variables -----------------

	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	UBuildingPlacerSettings* Settings;

	UPROPERTY(ReplicatedUsing=OnRep_BuildingToPlace)
	UBuildingSettings* BuildingToPlace = nullptr;

	UPROPERTY(Replicated)
	AMouseUtils* MouseUtils;

	// ----------------- Placing -----------------

public:
	void PlaceBuilding();

	bool IsPlacing() { return BuildingToPlace != nullptr; }

private:
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
