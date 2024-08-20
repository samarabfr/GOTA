// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CombatTile.h"


bool FCombatTile::operator==(const FCombatTile& Other) const
{
	return Equals(Other);
}

bool FCombatTile::Equals(const FCombatTile& Other) const
{
	return Tile == Other.Tile;
}
