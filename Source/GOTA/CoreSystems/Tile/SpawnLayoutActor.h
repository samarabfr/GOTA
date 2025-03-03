// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpawnLayoutActor.generated.h"

/**
 * An Actor to design a spawn layout. Create a blueprint that inherits from this class to start designing.
 * Add TileAssetComponents in the Blueprint to add Components
 */
UCLASS()
class GOTA_API ASpawnLayoutActor : public AActor
{
	GENERATED_BODY()

public:
	ASpawnLayoutActor();

};
