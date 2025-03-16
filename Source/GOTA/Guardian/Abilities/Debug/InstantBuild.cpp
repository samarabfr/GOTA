// Fill out your copyright notice in the Description page of Project Settings.


#include "InstantBuild.h"

#include "GOTA/Guardian/AbilityFramework/AbilityTarget.h"
#include "GOTA/Tile/Tile.h"
#include "GOTA/Tile/Building/Building.h"

void AInstantBuild::S_ActivateAbility(FAbilityTarget Target)
{
	if (Target.Tile.IsValid() && Target.Tile->GetBuilding())
	{
		Target.Tile->GetBuilding()->S_FinishConstruction();
	}
}

bool AInstantBuild::IsValidTarget(FAbilityTarget Target)
{
	return Target.Tile.IsValid() && Target.Tile->GetBuilding();
}
