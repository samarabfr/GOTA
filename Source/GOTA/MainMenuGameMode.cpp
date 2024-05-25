// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"

#include "EditorDirectories.h"

void AMainMenuGameMode::Travel(FString LevelPath)
{
	GetWorld()->ServerTravel(LevelPath + "?listen", TRAVEL_Absolute);
}

void AMainMenuGameMode::TravelClient(APlayerController* PlayerController, FString LevelPath)
{
	PlayerController->ClientTravel(LevelPath, TRAVEL_Absolute);
}
