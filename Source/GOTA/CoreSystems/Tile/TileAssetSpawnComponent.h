// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpawnPoint.h"
#include "TileAsset.h"
#include "Components/ActorComponent.h"
#include "TileAssetSpawnComponent.generated.h"

/**
 * A Component to use with SpawnLayoutActor to help designing SpawnLayouts.
 * Every instance of this component represents a SpawnLocation for a TileAsset.
 */
UCLASS(HideCategories=(
	"Variable", "Materials", "Component Tick", "Physics", "Collision", "MeshPainting",
	"Lighting", "Rendering", "HLOD", "Navigation", "Virtual Texture", "Tags", "Component Replication",
	"Cooking", "Events", "LOD", "Ray Tracing", "Texture Streaming", "Material Parameters", "Mobile",
	"Asset User Data", "Replication"), meta = (BlueprintSpawnableComponent))

class GOTA_API UTileAssetSpawnComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UTileAssetSpawnComponent();
	
	UPROPERTY(EditDefaultsOnly, Category = "_SpawnLayoutActor")
	ETileAssetCategory Category = ETileAssetCategory::None;
	
	UPROPERTY(EditDefaultsOnly, Category = "_SpawnLayoutActor")
	uint8 SpawnChance = 100;
	
	UPROPERTY(EditDefaultsOnly, Category = "_SpawnLayoutActor")
	TArray<UTileAsset*> ForcedAssets;

public:
	ETileAssetCategory GetTileAssetCategory() { return Category; }
	FSpawnPoint GetSpawnPoint();
};
