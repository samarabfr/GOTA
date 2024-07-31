// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementBalance.generated.h"

UCLASS(Blueprintable)
class GOTA_API USettlementBalance : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army spawning")
	int32 MinimumPopulationToSpawnArmy;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army spawning")
	int32 HighPopulationThreshold;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army spawning")
	float HighPopulationImpact;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army spawning")
	float AggressiveMoodMaximumImpact;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army spawning")
	float MinimumRatioOfPopulationJoiningArmy;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Army spawning")
	float MaximumRatioOfPopulationJoiningArmy;
};
