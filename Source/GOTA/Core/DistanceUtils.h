// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTAGameState.h"
#include "GameFramework/Actor.h"
#include "GOTA/TileMap/HexCoords.h"
#include "DistanceUtils.generated.h"

UCLASS()
class GOTA_API ADistanceUtils : public AActor
{
	GENERATED_BODY()
	ADistanceUtils();

virtual void BeginPlay() override;
	
	static const float ActiveTileRange;
	
	virtual void Tick(float DeltaSeconds) override;

private:
	FHexCoords LastCoords;
	TArray<FHexCoords> LastCoordsInRange;
	TWeakObjectPtr<AGOTAGameState> CachedGameState;
};