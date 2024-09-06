// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Terrain.h"
#include "GameplayTagContainer.h"
#include "GameplayTagRule.h"
#include "SpawnBias.h"
#include "GameFramework/Actor.h"
#include "TileAsset.h"
#include "TileAssetDA.generated.h"

UCLASS(BlueprintType)
class UTileAssetDA : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMesh* StaticMesh = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	ERotationMode RotationMode = ERotationMode::SpawnPointRotation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FSpawnBias SpawnBias;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FGameplayTagRule> GameplayTagRules;

	bool IsValidFor(const FGameplayTagContainer& GameplayTagContainer) const;

	int32 GetBiasAfterMultipliers(const FTerrain& Terrain) const;

	float GetRotationAfterMode() const;
};
