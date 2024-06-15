// Fill out your copyright notice in the Description page of Project Settings.


#include "GOTALobbyGameMode.h"



void AGOTALobbyGameMode::Travel(FString LevelPath)
{
	GetWorld()->ServerTravel(LevelPath + "?listen", TRAVEL_Absolute);
}