// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnPoint.h"
#include "TileAsset.h"
#include "GameFramework/Actor.h"
#include "TileAssetWithPosition.generated.h"

USTRUCT(BlueprintType)
struct FTileAssetWithPosition : public FTableRowBase
{
	GENERATED_BODY()

	FTileAssetWithPosition(FName TileAssetRowName_, const FSpawnPoint& SpawnPoint_);
	FTileAssetWithPosition();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Graphics")
	FName TileAssetRowName = "";

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Graphics")
	FSpawnPoint SpawnPoint;
};
