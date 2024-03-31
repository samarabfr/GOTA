// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexCoords.h"
#include "Tile.h"
#include "GameFramework/Actor.h"
#include "TileMap.generated.h"

UCLASS()
class GOTA_API ATileMap : public AActor
{
	GENERATED_BODY()

private:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector2D HexCoordsToWorldPos(FHexCoords HexCoords);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FHexCoords WorldPosToHexCoords(FVector2D Vector);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileMap")
	float GridSize;

private:
	UPROPERTY()
	TArray<ATile*> TileMap;

	UPROPERTY()
	int32 MapSize;
	
protected:
	UFUNCTION(BlueprintCallable)
	void Init(int32 Init_MapSize);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool DoesTileExist(FHexCoords HexCoords);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ATile* GetTile(FHexCoords HexCoords);

	UFUNCTION(BlueprintCallable)
	void AddTile(FHexCoords HexCoords, ATile* Tile);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ATile* Cpp_GetRandomTile();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<ATile*> Cpp_GetNeighboringTiles(ATile* Origin);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<ATile*> Cpp_GetPath(ATile* Start, ATile* End);
};