// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageBuildingAbility.h"

#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/Population.h"
#include "GOTA/CoreSystems/Tile/Tile.h"


void ADamageBuildingAbility::Use(ATile* Target, ATile* PlayerPosition)
{
	if (!CanBeUsed(Target, PlayerPosition) &&
		!Target->GetBuilding())
		return;
	if (Target->GetBuilding()->GetPopulation()->GetSize() > 0)
	{
		Target->GetBuilding()->GetPopulation()->S_DecreaseSize(1);
	}
	else
	{
		Target->Unbuild();
	}
}
