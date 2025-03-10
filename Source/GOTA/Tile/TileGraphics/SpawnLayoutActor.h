// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TileLayout.h"
#include "GameFramework/Actor.h"
#include "SpawnLayoutActor.generated.h"

/**
 * An Actor to design a spawn layout. Create a blueprint that inherits from this class to start designing.
 * Add TileAssetComponents in the Blueprint to add Components
 */
UCLASS(HideCategories=(
	"Actor Tick", "Replication", "Rendering", "Collision", "Actor", "Input", "HLOD", "Physics", "Events",
	"Level Instance", "Cooking", "World Partition", "Data Layers"))
class GOTA_API ASpawnLayoutActor : public AActor
{
	GENERATED_BODY()

	// ---------------------------------------- Lifecycle ----------------------------------------
public:
	ASpawnLayoutActor();

	// ---------------------------------------- Utility ----------------------------------------

private:
	UPROPERTY(EditDefaultsOnly, Category = "SpawnLayoutActor", meta = (ForceRebuildProperty))
	ETileLayout TileLayout = ETileLayout::Layout_01;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnLayoutActor")
	TArray<FGameplayTagRule> GameplayTagRules;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnLayoutActor")
	bool GuaranteedIfPossible = false;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnLayoutActor")
	FSpawnBias SpawnBias;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Hexagon;

public:
	/**
	 * The TileLayout should be set in the BlueprintClass
	 * @return the TileLayout where this SpawnLayout is allowed to be used
	 */
	ETileLayout GetTileLayout() { return TileLayout; }

	/**
	 * Generates a SpawnLayoutStruct based on this Blueprint
	 */
	FSpawnLayoutStruct GetSpawnLayoutStruct();
};
