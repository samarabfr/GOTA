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
	int32 BuildingDowngrade = 0;
	
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Combat")
	int32 BuildingPopKills = 0;
	
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Combat")
	int32 EnemyEntityKills = 0;
	
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Combat")
	int32 AlliedEntityKills = 0;

	int32 GetEntityKills(EAffiliation Affiliation);

	void SetEntityKills(int32 Kills, EAffiliation Affiliation);

	void ZeroNumbers();
	
	bool operator==(const FCombatTile& Other) const;
	 
	bool Equals(const FCombatTile& Other) const;
};
