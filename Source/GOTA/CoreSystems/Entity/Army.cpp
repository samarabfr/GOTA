// Fill out your copyright notice in the Description page of Project Settings.

#include "Army.h"

#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"

void AArmy::CalculateMovement()
{
	if (!IsTargetValid() || IsNextStepBlocked())
	{
		AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
		EAffiliation Enemy = GetAffiliation() == EAffiliation::Ally ? EAffiliation::Enemy : EAffiliation::Ally;
		SetPath(GameState->TileMap->GetPathToNearestAffiliatedBuilding(CurrentTile, Enemy, GetAffiliation()));
		if (GetPath().Num() > 0) Target = GetPath()[0];
	}
	Step();
}

AArmy::AArmy()
{
	PopCon = CreateDefaultSubobject<UPopulation>("Population Container");
//	PopCon->CombatValues->OnChanged.AddDynamic(this, &AArmy::CombatValuesChanged);
}

bool AArmy::IsTargetValid()
{
	// no target
	if (!Target) return false;
	// target has no building
	if (!Target->GetBuilding()) return false;
	// target is not claimed
	if (!Target->GetClaimant()) return false;
	// target is not claimed by the enemy
	if (Target->GetClaimant()->Affiliation == GetAffiliation()) return false;
	return true;
}

UCombatValues* AArmy::GetCombatValues() const
{
//	return PopCon->CombatValues;
	return nullptr;
}

void AArmy::KillIndividuals(int32 Kills)
{
	PopCon->S_DecreaseSize(Kills);
	if (PopCon->GetSize() <= 0)
		Kill();
}

void AArmy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AArmy::Init(EAffiliation Affiliation_, ATile* CurrentTile_, int32 MovementSpeed_)
{
	Super::Init(Affiliation_, CurrentTile_, MovementSpeed_);
	PopCon->S_DecreaseSize(100);
	PopCon->S_IncreaseSize(FMath::RandRange(3,10));
}
