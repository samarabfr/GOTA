// Fill out your copyright notice in the Description page of Project Settings.


#include "GOTALobbyGameState.h"

#include "Net/UnrealNetwork.h"

void AGOTALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AGOTALobbyGameState, IslandRadius);
	DOREPLIFETIME(AGOTALobbyGameState, NativesCount);
	DOREPLIFETIME(AGOTALobbyGameState, ColonistsCount);
	DOREPLIFETIME(AGOTALobbyGameState, LobbyPlayers);
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

void AGOTALobbyGameState::OnRep_LobbyPlayers()
{
	OnLobbyPlayersChanged.Broadcast();
}

ULobbyPlayer* AGOTALobbyGameState::CreateLobbyPlayer()
{
	ULobbyPlayer* LobbyPlayer = NewObject<ULobbyPlayer>(this);
	AddReplicatedSubObject(LobbyPlayer);
	LobbyPlayers.Add(LobbyPlayer);
	return LobbyPlayer;
}
