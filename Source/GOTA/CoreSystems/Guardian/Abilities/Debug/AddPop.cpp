// Fill out your copyright notice in the Description page of Project Settings.


#include "AddPop.h"
#include "GOTA/CoreSystems/Faction/Building/Population.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

void AAddPop::SRPC_ActivateAbility(FAbilityTarget Target)
{
	if (Target.Tile.IsValid() && Target.Tile->GetBuilding())
	{
		Target.Tile->GetBuilding()->GetPopulation()->S_IncreaseSize(1);
	}
}

bool AAddPop::IsValidTarget(FAbilityTarget Target)
{
	return Target.Tile.IsValid() && Target.Tile->GetBuilding();
}
