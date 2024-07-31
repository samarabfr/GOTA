// Fill out your copyright notice in the Description page of Project Settings.

#include "Army.h"
#include "GOTA/TileMap/Tile.h"
#include "Settlement.h"
#include "GOTA/CoreSystems/GOTAGameState.h"

void AArmy::CalculateMovement()
{
	if (!IsTargetValid() || IsNextStepBlocked())
	{
		AGOTAGameState* GameState = GetWorld()->GetGameState<AGOTAGameState>();
		EAffiliation Enemy = Affiliation == EAffiliation::Ally ? EAffiliation::Enemy : EAffiliation::Ally;
		Path = GameState->TileMap->GetPathToNearestAffiliatedBuilding(CurrentTile, Enemy);
	}
	Step();
}

bool AArmy::IsTargetValid() const
{
	// no target
	if(!Target) return false;
	// target has no building
	if(!Target->Building) return false;
	// target is not claimed
	if(!Target->GetClaimant()) return false;
	// target is not claimed by the enemy
	if(Target->GetClaimant()->Affiliation == Affiliation) return false;
	return true;
}
