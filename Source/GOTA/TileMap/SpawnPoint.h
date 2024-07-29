// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

USTRUCT(BlueprintType)
struct FSpawnPoint
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FVector LocationOnTile = FVector(0, 0, 0);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float DefaultRotation = 0.0f;
};
