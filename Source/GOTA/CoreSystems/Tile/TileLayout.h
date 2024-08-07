// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnPointLayout.h"
#include "GameFramework/Actor.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
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
	bool AllowNativesBuilding;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool AllowColonistBuilding;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool AllowNoBuilding;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool HasRiver;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<bool> RiverConnections;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<FSpawnPointLayout> SpawnPointsLayouts;
};
