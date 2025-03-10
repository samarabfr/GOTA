// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpawnPoint.h"
#include "SpawnLayout.generated.h"

class UTileAssetSpawnComponent;

USTRUCT()
struct FSpawnLayout
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FSpawnPoint MainBuilding;

	UPROPERTY(EditDefaultsOnly)
	TArray<FSpawnPoint> MainBuildings;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FSpawnPoint> Buildings;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FSpawnPoint> Trees;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FSpawnPoint> Forage;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FSpawnPoint> Props;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FSpawnPoint> Foliage;

	void AddSpawnPoint(ETileAssetCategory Category, FSpawnPoint SpawnPoint);
};