// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GOTA/Faction/Enums.h"
#include "CoreMinimal.h"
#include "SpawnPointLayout.h"
#include "GameFramework/Actor.h"
#include "TileLayout.generated.h"

USTRUCT(BlueprintType)
struct FTileLayout : public FTableRowBase
{
	GENERATED_BODY()

	FTileLayout();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UStaticMesh* HexagonMesh;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<EBiome> AllowedBiomes;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool AllowBuilding;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool AllowNoBuilding;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool AllowRiver;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<bool> RiverConnections;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<FSpawnPointLayout> SpawnPointsLayouts;
};
