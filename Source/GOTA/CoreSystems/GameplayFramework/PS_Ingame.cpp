// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Ingame.h"
#include "Net/UnrealNetwork.h"

void APS_Ingame::SetPlayerID(int32 NewPlayerID)
{
	GOTAPlayerID = NewPlayerID;
}

void APS_Ingame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APS_Ingame, GOTAPlayerID);
}