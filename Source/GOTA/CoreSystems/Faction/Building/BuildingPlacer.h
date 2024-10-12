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
	ABuildingPlacer();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void Init(AMouseUtils* InMouseUtils);

private:
	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	UBuildingPlacerSettings* Settings;
	
	UPROPERTY(ReplicatedUsing=OnRep_BuildingToPlace)
	UBuildingSettings* BuildingToPlace = nullptr;
	
	UPROPERTY(Replicated)
	AMouseUtils* MouseUtils;

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

	void ShowPlacingBuilding();
	void StopShowingPlacingBuilding();
	
public:
	void PlaceBuilding();

	bool IsPlacing() { return BuildingToPlace != nullptr; }

private:
	UFUNCTION()
	void RefreshPlaceability(ATile* NewTile);

	bool CanPlace(ATile* Tile);
};

