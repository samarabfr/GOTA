// Fill out your copyright notice in the Description page of Project Settings.


#include "InstantBuild.h"

#include "GOTA/CoreSystems/Tile/Tile.h"

void AInstantBuild::SRPC_ActivateAbility(FAbilityTarget Target)
{
	if (Target.Tile.IsValid() && Target.Tile->GetBuilding())
	{
		Target.Tile->GetBuilding()->FinishConstruction();
	}
	UE_LOG(LogTemp, Warning, TEXT("Instant Build Activated"))
}

bool AInstantBuild::IsValidTarget(FAbilityTarget Target)
{
	return Target.Tile.IsValid() && Target.Tile->GetBuilding();
}
