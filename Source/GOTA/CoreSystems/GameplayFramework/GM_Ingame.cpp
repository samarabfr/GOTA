// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Ingame.h"

#include "GOTAGameInstance.h"
#include "PC_Ingame.h"
#include "PS_Ingame.h"
#include "GameFramework/GameStateBase.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Tile/WorldGenerator.h"
#include "Kismet/GameplayStatics.h"

void AGM_Ingame::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	APS_Ingame* GOTAPlayerState = NewPlayer->GetPlayerState<APS_Ingame>();
	int32 PlayerID = GameState->PlayerArray.Num() - 1; //0-based index
	GOTAPlayerState->SetPlayerID(PlayerID);
}

void AGM_Ingame::Init()
{
	GOTAGameState = GetGameState<AGS_Ingame>();
}

void AGM_Ingame::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!GOTAGameState) return;
	if (GOTAGameState->ElapsedTurnTime >= GOTAGameState->MaxTurnTime)
	{
		CalculateTurn();
		GOTAGameState->SetElapsedTurnTimeMulticast(0);
	}
}

void AGM_Ingame::CreateWorld()
{
	GOTAGameState->TileMap = GetWorld()->SpawnActor<ATileMap>(TileMapClass);
	UGOTAGameInstance* GameInstance = GetGameInstance<UGOTAGameInstance>();
	UWorldGenerator* WorldGen = NewObject<UWorldGenerator>();
	WorldGen->Init(GOTAGameState->TileMap, GameInstance->IslandTileCount, GameInstance->ColonistsSettlementCount,
	               GameInstance->NativesSettlementCount);
	WorldGen->GenerateWorld();
}

void AGM_Ingame::CreateFactions()
{
	for(ATile* Start : GOTAGameState->TileMap->ColonistsStarts)
	{
		ASettlement* Settlement = GetWorld()->SpawnActor<ASettlement>(ColonistSettlementClass);
		Settlement->InitialStartingSetup(Start);
		GOTAGameState->ColonistsSettlements.Add(Settlement);
	}
	for(ATile* Start : GOTAGameState->TileMap->NativesStarts)
	{
		ASettlement* Settlement = GetWorld()->SpawnActor<ASettlement>(NativeSettlementClass);
		Settlement->InitialStartingSetup(Start);
		GOTAGameState->NativeSettlements.Add(Settlement);
	}
}

void AGM_Ingame::CreateGuardians()
{
	UGOTAGameInstance* GameInstance = GetGameInstance<UGOTAGameInstance>();
	FVector Location = FVector(0,0,1000);
	if(GameInstance->SelectedGuardian1)
	{
		AGuardian* Guardian = GetWorld()->SpawnActor<AGuardian>(GameInstance->SelectedGuardian1, Location,FRotator());
		GOTAGameState->Guardians.Add(Guardian);
	}
	if(GameInstance->SelectedGuardian2)
	{
		AGuardian* Guardian = GetWorld()->SpawnActor<AGuardian>(GameInstance->SelectedGuardian2, Location,FRotator());
		GOTAGameState->Guardians.Add(Guardian);
	}
	if(GameInstance->SelectedGuardian3)
	{
		AGuardian* Guardian = GetWorld()->SpawnActor<AGuardian>(GameInstance->SelectedGuardian3, Location,FRotator());
		GOTAGameState->Guardians.Add(Guardian);
	}
	if(GameInstance->SelectedGuardian4)
	{
		AGuardian* Guardian = GetWorld()->SpawnActor<AGuardian>(GameInstance->SelectedGuardian4, Location,FRotator());
		GOTAGameState->Guardians.Add(Guardian);
	}
}

void AGM_Ingame::InitialPlayerControllerPossession()
{
	GetNumPlayers();
	// this makes little sense, see ticket #104
	for (int i = 0; i < GetNumPlayers(); ++i)
	{
		APC_Ingame* PC = Cast<APC_Ingame>(UGameplayStatics::GetPlayerController(GetWorld(), i));
		PC->PossessGuardian(GOTAGameState->Guardians[i]);
	}
}

void AGM_Ingame::StartGame()
{
	GOTAGameState->ShouldTickTurnTime = true;
}

void AGM_Ingame::CalculateTurn()
{
	GOTAGameState->TurnCalculationStart();
	StartedCalculatingTurn = FDateTime::Now();
	// Settlement Turns
	for (ASettlement* Settlement : GOTAGameState->ColonistsSettlements)
	{
		Settlement->CalculateTurn();
	}
	for (ASettlement* Settlement : GOTAGameState->NativeSettlements)
	{
		Settlement->CalculateTurn();
	}
	// Combat Phase
	for (AEntity* Entity : GOTAGameState->TileEntities)
	{
		if(Entity->ShouldCombatTrigger()) Entity->TriggerCombat();
	}
	// Entity Movement
	for (AEntity* Entity : GOTAGameState->TileEntities)
	{
		Entity->CalculateMovement();
	}
	// Ecovalues
	GOTAGameState->TileMap->CalculateTurn();
	// Finished
	FTimespan TimeSpan = FDateTime::Now() - StartedCalculatingTurn;
	UE_LOG(LogTemp, Warning, TEXT("It took %d.%d ms to calculate the %d turn."),
	       TimeSpan.GetFractionMilli(),
	       TimeSpan.GetFractionMicro(),
	       GOTAGameState->TurnCounter)
	++GOTAGameState->TurnCounter;
	GOTAGameState->TurnCalculationEnd();
}
