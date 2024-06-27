// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HexCoords.h"
#include "HexCoordsFunctions.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API UHexCoordsFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HexCoords")
	static FVector2D HexCoordsToWorldPos(FHexCoords HexCoords);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HexCoords")
	static FHexCoords WorldPosToHexCoords(FVector2D Vector);
};