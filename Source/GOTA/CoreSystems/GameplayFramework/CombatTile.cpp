// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CombatTile.h"


int32 FCombatTile::GetEntityKills(EAffiliation Affiliation)
{
	if(Affiliation == EAffiliation::Ally)
		return AlliedEntityKills;
	return EnemyEntityKills;
}

void FCombatTile::SetEntityKills(int32 Kills, EAffiliation Affiliation)
{
	if(Affiliation == EAffiliation::Ally)
		AlliedEntityKills = Kills;
	else
		EnemyEntityKills = Kills;
}

void FCombatTile::ZeroNumbers()
{
	BuildingDowngrade = 0;
	BuildingPopKills = 0;
	EnemyEntityKills = 0;
	AlliedEntityKills = 0;
}

bool FCombatTile::operator==(const FCombatTile& Other) const
{
	return Equals(Other);
}

bool FCombatTile::Equals(const FCombatTile& Other) const
{
	return Tile == Other.Tile;
}
