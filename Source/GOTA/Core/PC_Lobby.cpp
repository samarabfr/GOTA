// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Lobby.h"

void APC_Lobby::TravelClient_Implementation(const FString& LevelPath)
{
	ClientTravel(LevelPath + "?listen", TRAVEL_Absolute);
}
