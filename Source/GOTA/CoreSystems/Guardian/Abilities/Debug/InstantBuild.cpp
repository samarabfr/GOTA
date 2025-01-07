// Fill out your copyright notice in the Description page of Project Settings.


#include "InstantBuild.h"

#include "GOTA/CoreSystems/Tile/Tile.h"

void AInstantBuild::S_ActivateAbility(FAbilityTarget Target)
{
	if (Target.Tile.IsValid() && Target.Tile->GetBuilding())
	{
		Target.Tile->GetBuilding()->FinishConstruction();
	}
}

bool AInstantBuild::IsValidTarget(FAbilityTarget Target)
{
	return Target.Tile.IsValid() && Target.Tile->GetBuilding();
}
