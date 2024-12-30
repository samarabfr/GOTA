// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulatedPlayer.h"

#include "GOTA/CoreSystems/Tile/Tile.h"
#include "Net/UnrealNetwork.h"

ASimulatedPlayer::ASimulatedPlayer()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ASimulatedPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
}

void ASimulatedPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HasAuthority())
		S_Tick(DeltaSeconds);
	else
		C_Tick(DeltaSeconds);
}

void ASimulatedPlayer::S_Init()
{
}

void ASimulatedPlayer::S_Tick(const float DeltaSeconds)
{
}

void ASimulatedPlayer::C_Tick(const float DeltaSeconds)
{
}

void ASimulatedPlayer::BeginDestroy()
{
	Super::BeginDestroy();
}