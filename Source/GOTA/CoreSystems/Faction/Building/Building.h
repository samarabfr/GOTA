// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Population.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "Building.generated.h"

class UBuildingDataAsset;

UCLASS(Blueprintable)
class GOTA_API UBuilding : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	UBuilding();

public:
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Building")
	UBuildingDataAsset* DataAsset;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Building")
	UPopulation* Population;
};
