// Fill out your copyright notice in the Description page of Project Settings.


#include "GOTALobbyGameState.h"

void AGOTALobbyGameState::PlayersChanged_Implementation()
{
	OnPlayersChanged.Broadcast();
}
