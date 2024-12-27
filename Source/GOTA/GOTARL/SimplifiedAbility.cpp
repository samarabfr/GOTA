// Fill out your copyright notice in the Description page of Project Settings.


#include "SimplifiedAbility.h"

#include "GOTA/CoreSystems/Tile/Tile.h"

ASimplifiedAbility::ASimplifiedAbility()
{
}

void ASimplifiedAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ASimplifiedAbility::S_Init()
{
}

void ASimplifiedAbility::S_Tick(const float DeltaSeconds)
{
	if (CooldownLeft > 0.0f)
	{
		CooldownLeft -= DeltaSeconds;
	}
	if (CooldownLeft < 0.0f)
	{
		CooldownLeft = 0.0f;
	}
}

void ASimplifiedAbility::C_Tick(const float DeltaSeconds)
{
}

void ASimplifiedAbility::BeginDestroy()
{
	Super::BeginDestroy();
}

bool ASimplifiedAbility::CanBeUsed(const ATile* Target, const ATile* PlayerPosition) const
{
	return CooldownLeft == 0.0f &&
		Target &&
		PlayerPosition &&
		PlayerPosition->GetTileDistanceTo(Target) <= TileRange;
}

void ASimplifiedAbility::Use(ATile* Target, ATile* PlayerPosition)
{
}
