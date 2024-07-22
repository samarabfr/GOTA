// Fill out your copyright notice in the Description page of Project Settings.


#include "GOTALobbyGameState.h"

#include "Net/UnrealNetwork.h"

void AGOTALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AGOTALobbyGameState, IslandRadius);
	DOREPLIFETIME(AGOTALobbyGameState, NativesCount);
	DOREPLIFETIME(AGOTALobbyGameState, ColonistsCount);
	DOREPLIFETIME(AGOTALobbyGameState, LobbyPlayer1);
	DOREPLIFETIME(AGOTALobbyGameState, LobbyPlayer2);
	DOREPLIFETIME(AGOTALobbyGameState, LobbyPlayer3);
	DOREPLIFETIME(AGOTALobbyGameState, LobbyPlayer4);
}

void AGOTALobbyGameState::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		AddReplicatedSubObject(LobbyPlayer1);
		AddReplicatedSubObject(LobbyPlayer2);
		AddReplicatedSubObject(LobbyPlayer3);
		AddReplicatedSubObject(LobbyPlayer4);
	}
}

AGOTALobbyGameState::AGOTALobbyGameState()
{
	bReplicateUsingRegisteredSubObjectList = true;
	LobbyPlayer1 = CreateDefaultSubobject<ULobbyPlayer>(TEXT("Lobby Player 1"));
	LobbyPlayer2 = CreateDefaultSubobject<ULobbyPlayer>(TEXT("Lobby Player 2"));
	LobbyPlayer3 = CreateDefaultSubobject<ULobbyPlayer>(TEXT("Lobby Player 3"));
	LobbyPlayer4 = CreateDefaultSubobject<ULobbyPlayer>(TEXT("Lobby Player 4"));
}

void AGOTALobbyGameState::SetIslandRadius(int32 NewIslandRadius)
{
	IslandRadius = NewIslandRadius;
	OnIslandRadiusChanged.Broadcast(IslandRadius);
}

void AGOTALobbyGameState::OnRep_IslandRadius()
{
	OnIslandRadiusChanged.Broadcast(IslandRadius);
}

void AGOTALobbyGameState::SetNativesCount(int32 NewNativesCount)
{
	NativesCount = NewNativesCount;
	OnNativesCountChanged.Broadcast(NativesCount);
}

void AGOTALobbyGameState::OnRep_NativesCount()
{
	OnNativesCountChanged.Broadcast(NativesCount);
}

void AGOTALobbyGameState::SetColonistsCount(int32 NewColonistsCount)
{
	ColonistsCount = NewColonistsCount;
	OnColonistsCountChanged.Broadcast(ColonistsCount);
}

void AGOTALobbyGameState::OnRep_ColonistsCount()
{
	OnColonistsCountChanged.Broadcast(ColonistsCount);
}

void AGOTALobbyGameState::SetSelectedGuardian_Implementation(TSubclassOf<AGuardian> Guardian, ULobbyPlayer* LobbyPlayer)
{
	if(!LobbyPlayer) return;
	
	LobbyPlayer->SetSelectedGuardian(Guardian);
}
