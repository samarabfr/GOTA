// Fill out your copyright notice in the Description page of Project Settings.


#include "AddPop.h"

#include "GOTA/Guardian/AbilityFramework/AbilityTarget.h"
#include "GOTA/Tile/Building/Population.h"
#include "GOTA/Tile/Tile.h"
#include "GOTA/Tile/Building/Building.h"

void AAddPop::S_ActivateAbility(FAbilityTarget Target)
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
