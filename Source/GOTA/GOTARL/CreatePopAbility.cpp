// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatePopAbility.h"

#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/Population.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

ACreatePopAbility::ACreatePopAbility()
{
	Cooldown = 7.0f;
}

void ACreatePopAbility::Use(ATile* Target, ATile* PlayerPosition)
{
	if (!CanBeUsed(Target, PlayerPosition) ||
		!Target->GetBuilding())
		return;
	Target->GetBuilding()->GetPopulation()->S_IncreaseSize(1);
	ActivateCooldown();
}
