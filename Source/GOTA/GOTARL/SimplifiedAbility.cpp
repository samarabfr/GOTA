// Fill out your copyright notice in the Description page of Project Settings.


#include "SimplifiedAbility.h"

#include "GOTA/CoreSystems/Tile/Tile.h"
#include "Net/UnrealNetwork.h"

ASimplifiedAbility::ASimplifiedAbility()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ASimplifiedAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
}

void ASimplifiedAbility::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HasAuthority())
		S_Tick(DeltaSeconds);
	else
		C_Tick(DeltaSeconds);
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

void ASimplifiedAbility::ActivateCooldown()
{
	CooldownLeft = Cooldown;
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
