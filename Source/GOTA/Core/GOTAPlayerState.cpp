// Fill out your copyright notice in the Description page of Project Settings.


#include "GOTAPlayerState.h"
#include "Net/UnrealNetwork.h"

void AGOTAPlayerState::SetPlayerID(int32 NewPlayerID)
{
	GOTAPlayerID = NewPlayerID;
}

void AGOTAPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGOTAPlayerState, GOTAPlayerID);
}