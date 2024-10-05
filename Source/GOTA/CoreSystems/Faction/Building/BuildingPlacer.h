// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BuildingPlacer.generated.h"

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

	bool bIsPlacingBuilding = false;

	UPROPERTY()
	UBuildingSettings* BuildingToPlace = nullptr;

	UPROPERTY(Replicated)
	AMouseUtils* MouseUtils;

public:
	void StartPlacingBuilding(UBuildingSettings* Building);

	void StopPlacingBuilding();

	void PlaceBuilding();

	bool IsPlacing() { return bIsPlacingBuilding; }
};
