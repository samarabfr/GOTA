// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnPoint.h"
#include "GameFramework/Actor.h"
#include "SpawnPointLayout.generated.h"

USTRUCT(BlueprintType)
struct FSpawnPointLayout
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FSpawnPoint MainBuilding;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<FSpawnPoint> Trees;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<FSpawnPoint> Buildings;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<FSpawnPoint> Props;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<FSpawnPoint> Forage;
};
