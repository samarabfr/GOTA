// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexCoords.h"
#include "Engine/DataAsset.h"
#include "TestRiverDataAsset.generated.h"

UCLASS()
class GOTA_API UTestRiver : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	TArray<FHexCoords> RiverTiles;
	
};
