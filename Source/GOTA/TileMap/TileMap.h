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
	
public:
	static int32 MapOffset;
	
private:
	UPROPERTY(Replicated)
	TArray<ATile*> Tiles;
	
protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MapRadius)
	int32 MapRadius;
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	int32 MapSize;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileMap")
	void Init(int32 Init_MapRadius);

	UFUNCTION()
	void OnRep_MapRadius() const;
	
private:
	ATile*** TilesArray;
	void InitializeArray();
	
protected:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileMap")
	bool TryAddTile(FHexCoords HexCoords, ATile* Tile);
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="TileMap")
	ATile* GetTile(FHexCoords HexCoords);

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintAuthorityOnly, Category="TileMap")
	ATile* GetTileFast(FHexCoords HexCoords);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="TileMap")
	bool DoesTileExist(FHexCoords HexCoords);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="TileMap")
	ATile* GetRandomTile();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="TileMap")
	void GenerateTilesInAHexagon();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileMap")
	void CalculateTurn();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="TileMap")
	TArray<ATile*> GetPath(ATile* Start, ATile* End, EAffiliation Affiliation);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileMap")
	TArray<ATile*> GetPathToNearestAffiliatedBuilding(ATile* Start, EAffiliation Affiliation);
};