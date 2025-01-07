// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability.h"

AAbility::AAbility()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = false;
	SetNetUpdateFrequency(1.0f);

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 1.0f;
}

void AAbility::SRPC_ActivateAbility_Implementation(FAbilityTarget Target)
{
	S_ActivateAbility(Target);
}

void AAbility::ActivateAbility(FAbilityTarget Target)
{
	SRPC_ActivateAbility(Target);
}
