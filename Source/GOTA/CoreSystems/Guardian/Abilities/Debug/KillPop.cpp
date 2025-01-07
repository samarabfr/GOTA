// Fill out your copyright notice in the Description page of Project Settings.


#include "KillPop.h"
#include "GOTA/CoreSystems/Faction/Building/Population.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

void AKillPop::SRPC_ActivateAbility(FAbilityTarget Target)
{
	if (Target.Tile.IsValid() && Target.Tile->GetBuilding())
	{
		Target.Tile->GetBuilding()->GetPopulation()->S_DecreaseSize(1);
	}
}

bool AKillPop::IsValidTarget(FAbilityTarget Target)
{
	return Target.Tile.IsValid() && Target.Tile->GetBuilding();
}
