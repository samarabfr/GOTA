// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Terrain.h"
#include "GameplayTagContainer.h"
#include "GameplayTagRule.h"
#include "SpawnBias.h"
#include "TileAsset.generated.h"

UENUM()
enum class ERotationMode : uint8
{
	Default UMETA(DisplayName = "Default Spawn Point Rotation"),
	Random90Degree UMETA(DisplayName = "90 Degree Random"),
	Random360Degree UMETA(DisplayName = "360 Degree Random"),
};

UCLASS(BlueprintType)
class UTileAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* MeshFinished = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* MeshUnfinished = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* MeshDestroyed = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	ERotationMode RotationMode = ERotationMode::Default;

	UPROPERTY(EditDefaultsOnly)
	FSpawnBias SpawnBias;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTagRule> GameplayTagRules;

	bool IsValidFor(const FGameplayTagContainer& GameplayTagContainer) const;

	int32 GetBiasAfterMultipliers(const FTerrain& Terrain) const;

	float GetRotationAfterMode() const;
};
