// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "DistanceUtils.generated.h"

UCLASS()
class GOTA_API ADistanceUtils : public AActor
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	static const float ActiveTileRange;

protected:
	UFUNCTION(BlueprintCallable)
	void UpdateDistanceToTiles();
	
private:
	FHexCoords LastCoords;
	TArray<FHexCoords> LastCoordsInRange;
	TWeakObjectPtr<AGS_Ingame> CachedGameState;
};
