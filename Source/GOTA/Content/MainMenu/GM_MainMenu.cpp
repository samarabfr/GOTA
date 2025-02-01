// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_MainMenu.h"

#include "Engine/LevelStreamingDynamic.h"

void AGM_MainMenu::StartGame(const bool StartAsListenServer)
{
	if (!Level) return;
	const FString IslandPath = Level->GetOutermost()->GetName();
	const FString Path = StartAsListenServer ? IslandPath + "?listen" : IslandPath;
	GetWorld()->ServerTravel(Path, TRAVEL_Absolute);
}

void AGM_MainMenu::JoinGame(const FString IP)
{	
	if (!Level) return;
	const FString IslandPath = Level->GetOutermost()->GetName();
	GetWorld()->GetFirstPlayerController()->ClientTravel(IP + IslandPath, TRAVEL_Absolute);
}
