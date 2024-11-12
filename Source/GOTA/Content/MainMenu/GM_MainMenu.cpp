// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_MainMenu.h"

void AGM_MainMenu::StartGame(const bool StartAsListenServer)
{
	const FString Path = StartAsListenServer ? IslandPath + "?listen" : IslandPath;
	GetWorld()->ServerTravel(Path, TRAVEL_Absolute);
}

void AGM_MainMenu::JoinGame(const FString IP)
{
	GetWorld()->GetFirstPlayerController()->ClientTravel(IP + IslandPath, TRAVEL_Absolute);
}
