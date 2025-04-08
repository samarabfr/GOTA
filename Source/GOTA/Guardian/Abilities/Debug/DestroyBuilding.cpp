// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyBuilding.h"

#include "GOTA/Guardian/AbilityTarget.h"
#include "GOTA/Tile/Tile.h"
#include "GOTA/Tile/Building/Building.h"

void ADestroyBuilding::S_ActivateAbility(FAbilityTarget Target)
{
	if (IsValidTarget(Target))
	{
		Target.Tile->S_Unbuild();
	}
}

bool ADestroyBuilding::IsValidTarget(FAbilityTarget Target)
{
	return Target.Tile.IsValid() && Target.Tile->GetBuilding();
}
