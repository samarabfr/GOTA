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

void AGOTALobbyGameState::IslandRadiusOnRep(int32 NewIslandRadius)
{
	OnIslandRadiusChanged.Broadcast(NewIslandRadius);
}

void AGOTALobbyGameState::NativesCountOnRep(int32 NewNativesCount)
{
	OnNativesCountChanged.Broadcast(NewNativesCount);
}

void AGOTALobbyGameState::ColonistsCountOnRep(int32 NewColonistsCount)
{
	OnColonistsCountChanged.Broadcast(NewColonistsCount);
}
