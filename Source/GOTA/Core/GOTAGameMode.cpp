// Fill out your copyright notice in the Description page of Project Settings.


#include "GOTAGameMode.h"

void AGOTAGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	NewPlayer->ClientTravel("/Game/Content/Core/Island", TRAVEL_Absolute);
}
