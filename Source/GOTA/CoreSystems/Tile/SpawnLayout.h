// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnPoint.h"
#include "GameFramework/Actor.h"
#include "SpawnLayout.generated.h"

USTRUCT()
struct FSpawnLayout
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FSpawnPoint MainBuilding;

	UPROPERTY(EditDefaultsOnly)
	TArray<FSpawnPoint> Trees;

	UPROPERTY(EditDefaultsOnly)
	TArray<FSpawnPoint> Buildings;

	UPROPERTY(EditDefaultsOnly)
	TArray<FSpawnPoint> Props;

	UPROPERTY(EditDefaultsOnly)
	TArray<FSpawnPoint> Forage;

	UPROPERTY(EditDefaultsOnly)
	TArray<FSpawnPoint> Foliage;
};
