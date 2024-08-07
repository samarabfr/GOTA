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
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName Name;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 SpawnBias = 100;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FSpawnPoint MainBuilding;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FSpawnPoint> Trees;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FSpawnPoint> Buildings;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FSpawnPoint> Props;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FSpawnPoint> Forage;
};
