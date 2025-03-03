// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TileAssetSpawnComponent.generated.h"

/**
 * A Component to use with SpawnLayoutActor to help designing SpawnLayouts.
 * Every instance of this component represents a SpawnLocation for a TileAsset.
 */
UCLASS()
class GOTA_API UTileAssetSpawnComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UTileAssetSpawnComponent();
};
