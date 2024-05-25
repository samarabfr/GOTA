// Fill out your copyright notice in the Description page of Project Settings.


#include "GOTAGameMode.h"


void AGOTAGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(!NewPlayer->IsLocalPlayerController())
	{
		NewPlayer->ClientTravel("/Game/Core/Island", TRAVEL_Absolute);
	}
}
