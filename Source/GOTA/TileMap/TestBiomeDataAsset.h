// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexCoords.h"
#include "Engine/DataAsset.h"
#include "GOTA/Faction/Enums.h"
#include "TestBiomeDataAsset.generated.h"

UCLASS()
class GOTA_API UTestBiomeDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	TMap<FHexCoords, EBiome> BiomeTiles;
	
};
