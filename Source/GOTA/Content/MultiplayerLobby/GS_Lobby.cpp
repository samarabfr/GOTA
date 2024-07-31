// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_Lobby.h"

#include "Net/UnrealNetwork.h"

void AGS_Lobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AGS_Lobby, IslandRadius);
	DOREPLIFETIME(AGS_Lobby, NativesCount);
	DOREPLIFETIME(AGS_Lobby, ColonistsCount);
	DOREPLIFETIME(AGS_Lobby, LobbyPlayer1);
	DOREPLIFETIME(AGS_Lobby, LobbyPlayer2);
	DOREPLIFETIME(AGS_Lobby, LobbyPlayer3);
	DOREPLIFETIME(AGS_Lobby, LobbyPlayer4);
}

void AGS_Lobby::BeginPlay()
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

AGS_Lobby::AGS_Lobby()
{
	bReplicateUsingRegisteredSubObjectList = true;
	LobbyPlayer1 = CreateDefaultSubobject<ULobbyPlayer>(TEXT("Lobby Player 1"));
	LobbyPlayer2 = CreateDefaultSubobject<ULobbyPlayer>(TEXT("Lobby Player 2"));
	LobbyPlayer3 = CreateDefaultSubobject<ULobbyPlayer>(TEXT("Lobby Player 3"));
	LobbyPlayer4 = CreateDefaultSubobject<ULobbyPlayer>(TEXT("Lobby Player 4"));
}

void AGS_Lobby::SetIslandRadius(int32 NewIslandRadius)
{
	IslandRadius = NewIslandRadius;
	OnIslandRadiusChanged.Broadcast(IslandRadius);
}

void AGS_Lobby::OnRep_IslandRadius()
{
	OnIslandRadiusChanged.Broadcast(IslandRadius);
}

void AGS_Lobby::SetNativesCount(int32 NewNativesCount)
{
	NativesCount = NewNativesCount;
	OnNativesCountChanged.Broadcast(NativesCount);
}

void AGS_Lobby::OnRep_NativesCount()
{
	OnNativesCountChanged.Broadcast(NativesCount);
}

void AGS_Lobby::SetColonistsCount(int32 NewColonistsCount)
{
	ColonistsCount = NewColonistsCount;
	OnColonistsCountChanged.Broadcast(ColonistsCount);
}

void AGS_Lobby::OnRep_ColonistsCount()
{
	OnColonistsCountChanged.Broadcast(ColonistsCount);
}

void AGS_Lobby::SetSelectedGuardian_Implementation(TSubclassOf<AGuardian> Guardian, ULobbyPlayer* LobbyPlayer)
{
	if(!LobbyPlayer) return;
	
	LobbyPlayer->SetSelectedGuardian(Guardian);
}
