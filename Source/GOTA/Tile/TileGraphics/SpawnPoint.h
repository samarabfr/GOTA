// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TileAsset.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

USTRUCT(BlueprintType)
struct FSpawnPoint
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FVector LocationOnTile = FVector(0, 0, 0);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Rotation = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	uint8 SpawnChance = 100;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,
		meta = (ToolTip = "Picks full random out of this Array if it isn't empty."))
	TArray<UTileAsset*> ForcedAssets;
};
