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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	ATileMap();
private:
	UPROPERTY(Replicated)
	TArray<ATile*> TileMap;

	UPROPERTY(Replicated)
	int32 MapSize;

public:
	UPROPERTY(EditAnywhere, Category = "TileMap")
	float GridSize;
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="TileMap")
	void GenerateCircle(const int32 Radius);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileMap")
	void CalculateTurn();
	
protected:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileMap")
	void Init(int32 Init_MapSize);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileMap")
	void AddTile(FHexCoords HexCoords, ATile* Tile);

public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="TileMap")
	bool DoesTileExist(FHexCoords HexCoords);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="TileMap")
	ATile* GetTile(FHexCoords HexCoords);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="TileMap")
	ATile* GetRandomTile();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="TileMap")
	TArray<ATile*> GetPath(ATile* Start, ATile* End);
};