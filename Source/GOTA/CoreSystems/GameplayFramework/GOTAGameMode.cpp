// Fill out your copyright notice in the Description page of Project Settings.


#include "GOTAGameMode.h"
#include "GOTAPlayerState.h"
#include "GameFramework/GameStateBase.h"

void AGOTAGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	AGOTAPlayerState* GOTAPlayerState = NewPlayer->GetPlayerState<AGOTAPlayerState>();
	int32 PlayerID = GameState->PlayerArray.Num() - 1; //0-based index
	GOTAPlayerState->SetPlayerID(PlayerID);
}