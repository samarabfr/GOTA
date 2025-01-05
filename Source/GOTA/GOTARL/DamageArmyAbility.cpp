// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageArmyAbility.h"

#include "GOTA/CoreSystems/Entity/Army.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

ADamageArmyAbility::ADamageArmyAbility()
{
	Cooldown = 7.0f;
}

void ADamageArmyAbility::Use(ATile* Target, ATile* PlayerPosition)
{
	if (!CanBeUsed(Target, PlayerPosition) ||
		!Target->GetArmy())
		return;
	Target->GetArmy()->S_ArmyTakeDamage(Damage);
	ActivateCooldown();
}
