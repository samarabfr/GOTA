// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Ingame.h"
#include "PS_Ingame.h"
#include "GameFramework/GameStateBase.h"
#include "GOTA/CoreSystems/Tile/WorldGenerator.h"

void AGM_Ingame::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	APS_Ingame* GOTAPlayerState = NewPlayer->GetPlayerState<APS_Ingame>();
	int32 PlayerID = GameState->PlayerArray.Num() - 1; //0-based index
	GOTAPlayerState->SetPlayerID(PlayerID);
}

void AGM_Ingame::CreateWorld()
{
	UWorldGenerator* WorldGen = NewObject<UWorldGenerator>();
	WorldGen->Init(GOTAGameState->TileMap, 600, 6, 4);
	WorldGen->GenerateWorld();
}
