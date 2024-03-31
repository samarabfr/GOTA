// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexCoords.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class GOTA_API ATile : public AActor
{
	GENERATED_BODY()

	ATile();

public:
	UPROPERTY(BlueprintReadOnly)
	FHexCoords HexCoords;

	UPROPERTY(BlueprintReadWrite)
	bool IsWalkable;
};
