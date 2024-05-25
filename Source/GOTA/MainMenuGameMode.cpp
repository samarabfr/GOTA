// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"

void AMainMenuGameMode::Travel(FString LevelPath)
{
	GetWorld()->ServerTravel(LevelPath, ETravelType::TRAVEL_Absolute);
}
