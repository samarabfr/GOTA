// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileMap.generated.h"

UCLASS()
class GOTA_API ATileMap : public AActor
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector2D HexCoordsToWorldPos(FHexCoords HexCoords);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FHexCoords WorldPosToHexCoords(FVector2D Vector);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileMap")
	float GridSize;
};