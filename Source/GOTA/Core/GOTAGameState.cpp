// Fill out your copyright notice in the Description page of Project Settings.


#include "GOTAGameState.h"
#include "Net/UnrealNetwork.h"

//Unreal Engine Mystery Code
void AGOTAGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AGOTAGameState, MaxTurnTime);
	DOREPLIFETIME(AGOTAGameState, ElapsedTurnTime);
	DOREPLIFETIME(AGOTAGameState, IsCalculatingTurn);
}

void AGOTAGameState::AddFaction(AFaction* NewFaction)
{
	Factions.Add(NewFaction);
}

void AGOTAGameState::AddTileEntity(ATileEntity* NewTileEntity)
{
	TileEntities.Add(NewTileEntity);
}

float AGOTAGameState::GetElapsedTurnTime()
{
	return ElapsedTurnTime;
}

void AGOTAGameState::SetElapsedTurnTime(float NewValue)
{
	ElapsedTurnTime = NewValue;
	TurnTimerChanged.Broadcast(ElapsedTurnTime, MaxTurnTime);
}

void AGOTAGameState::OnRep_ElapsedTurnTime(float NewValue)
{
	TurnTimerChanged.Broadcast(ElapsedTurnTime, MaxTurnTime);
}

void AGOTAGameState::CallCalculationStart()
{
	TurnCalculationStart.Broadcast();
}

void AGOTAGameState::CallCalculationEnd()
{
	TurnCalculationEnd.Broadcast();
}
