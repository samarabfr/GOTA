// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DistanceUtilsSettings.generated.h"

UCLASS()
class GOTA_API UDistanceUtilsSettings : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int ActiveTileRangeInTiles;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MinDistanceToCombatInCM;
};
