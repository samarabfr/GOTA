// Fill out your copyright notice in the Description page of Project Settings.


#include "GOTALobbyGameState.h"

#include "Net/UnrealNetwork.h"

void AGOTALobbyGameState::PlayersChanged_Implementation()
{
	OnPlayersChanged.Broadcast();
}

void AGOTALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AGOTALobbyGameState, IslandRadius);
	DOREPLIFETIME(AGOTALobbyGameState, NativesCount);
	DOREPLIFETIME(AGOTALobbyGameState, ColonistsCount);
}

void AGOTALobbyGameState::OnRep_IslandRadius()
{
	OnIslandRadiusChanged.Broadcast(IslandRadius);
}

void AGOTALobbyGameState::OnRep_NativesCount()
{
	OnNativesCountChanged.Broadcast(NativesCount);
}

void AGOTALobbyGameState::OnRep_ColonistsCount()
{
	OnColonistsCountChanged.Broadcast(ColonistsCount);
}
