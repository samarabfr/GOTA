// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Lobby.h"



void AGM_Lobby::Travel(FString LevelPath)
{
	GetWorld()->ServerTravel(LevelPath + "?listen", TRAVEL_Absolute);
}
