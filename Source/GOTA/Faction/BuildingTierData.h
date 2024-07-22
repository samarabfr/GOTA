// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProductionType.h"
#include "BuildingTierData.generated.h"

USTRUCT(BlueprintType)
struct GOTA_API FBuildingTierData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	int32 WoodCost = -1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	int32 StoneCost = -1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	int32 Housing = -1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	EProductionType ProductionType = EProductionType::MAX;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	int32 PopulationThreshold = -1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	int32 ProductionPerThreshold = -1;
};
