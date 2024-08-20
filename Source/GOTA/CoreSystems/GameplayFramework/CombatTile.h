// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "CombatTile.generated.h"

USTRUCT(BlueprintType)
struct FCombatTile
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Combat")
	ATile* Tile = nullptr;
	
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Combat")
	int32 BuildingsKills = 0;
	
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Combat")
	int32 EnemyKills = 0;
	
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Combat")
	int32 AlliedKills = 0;
	
	bool operator==(const FCombatTile& Other) const;
	 
	bool Equals(const FCombatTile& Other) const;
};
