// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_MainMenu.h"

void AGM_MainMenu::Travel(FString LevelPath)
{
	GetWorld()->ServerTravel(LevelPath + "?listen", TRAVEL_Absolute);
}

void AGM_MainMenu::TravelClient(APlayerController* PlayerController, FString LevelPath)
{
	PlayerController->ClientTravel(LevelPath, TRAVEL_Absolute);
}
