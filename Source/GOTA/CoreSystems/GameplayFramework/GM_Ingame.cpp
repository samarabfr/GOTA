// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Ingame.h"


#include "GOTAGameInstance.h"
#include "PC_Ingame.h"
#include "PS_Ingame.h"
#include "GameFramework/GameStateBase.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Tile/WorldGenerator.h"
#include "Kismet/GameplayStatics.h"

AGM_Ingame::AGM_Ingame()
{
	ConstructorHelpers::FObjectFinder<UGameBalanceDataAsset> DataAssetFinder(
		TEXT("/Game/CoreSystems/GameplayFramework/DA_GameBalance"));
	if (DataAssetFinder.Succeeded())
	{
		GameBalance = DataAssetFinder.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load GameBalance DataAsset Inside GameMode!"));
	}
}

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
	// Start the Game paused
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetPause(true);
}

void AGM_Ingame::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!GOTAGameState) return;
	CheckGameEndingConditions();
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
	GOTAGameState->TileMap->Init();
}

void AGM_Ingame::CreateFactions()
{
	for (ATile* Start : GOTAGameState->TileMap->ColonistsStarts)
	{
		ASettlement* Settlement = GetWorld()->SpawnActor<ASettlement>(ColonistSettlementClass);
		Settlement->InitialStartingSetup(Start);
		GOTAGameState->ColonistsSettlements.Add(Settlement);
	}
	for (ATile* Start : GOTAGameState->TileMap->NativesStarts)
	{
		ASettlement* Settlement = GetWorld()->SpawnActor<ASettlement>(NativeSettlementClass);
		Settlement->InitialStartingSetup(Start);
		GOTAGameState->NativeSettlements.Add(Settlement);
	}
}

void AGM_Ingame::CreateGuardians()
{
	UGOTAGameInstance* GameInstance = GetGameInstance<UGOTAGameInstance>();
	FVector Location = FVector(0, 0, 1000);
	if (GameInstance->SelectedGuardian1)
	{
		AGuardian* Guardian = GetWorld()->SpawnActor<AGuardian>(GameInstance->SelectedGuardian1, Location, FRotator());
		GOTAGameState->Guardians.Add(Guardian);
	}
	if (GameInstance->SelectedGuardian2)
	{
		AGuardian* Guardian = GetWorld()->SpawnActor<AGuardian>(GameInstance->SelectedGuardian2, Location, FRotator());
		GOTAGameState->Guardians.Add(Guardian);
	}
	if (GameInstance->SelectedGuardian3)
	{
		AGuardian* Guardian = GetWorld()->SpawnActor<AGuardian>(GameInstance->SelectedGuardian3, Location, FRotator());
		GOTAGameState->Guardians.Add(Guardian);
	}
	if (GameInstance->SelectedGuardian4)
	{
		AGuardian* Guardian = GetWorld()->SpawnActor<AGuardian>(GameInstance->SelectedGuardian4, Location, FRotator());
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
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	PC->SetPause(false);
	UE_LOG(LogTemp, Warning, TEXT("test: %d"), IsPaused())
}

void AGM_Ingame::CalculateTurn()
{
	GOTAGameState->TurnCalculationStart();
	StartedCalculatingTurn = FDateTime::Now();
	// Combat Phase
	GOTAGameState->CombatSystem->TriggerAllCombats();
	// Settlement Turns
	for (ASettlement* Settlement : GOTAGameState->ColonistsSettlements)
	{
		Settlement->CalculateTurn();
	}
	for (ASettlement* Settlement : GOTAGameState->NativeSettlements)
	{
		Settlement->CalculateTurn();
	}
	// Entity Movement
	for (AEntity* Entity : GOTAGameState->TileEntities)
	{
		Entity->CalculateMovement();
	}
	// Check for combats next round
	for (AEntity* Entity : GOTAGameState->TileEntities)
	{
		if (Entity->ShouldCombatTrigger())
			GOTAGameState->CombatSystem->RegisterCombat(Entity->CurrentTile);
	}
	// Ecovalues
	GOTAGameState->TileMap->CalculateTurn();
	// Finished
	FTimespan TimeSpan = FDateTime::Now() - StartedCalculatingTurn;
	UE_LOG(LogTemp, Warning, TEXT("It took %d ms to calculate the %d turn."),
	       TimeSpan.GetFractionMilli(),
	       GOTAGameState->TurnCounter)
	GOTAGameState->IncreaseTurnCounter();
	GOTAGameState->TurnCalculationEnd();
}

void AGM_Ingame::CheckGameEndingConditions()
{
	if (GOTAGameState->GameEnded) return;

	// based on SettlementPop
	int32 ColonialPop = GOTAGameState->TotalColonialPopulation->Population.Size;
	int32 NativePop = GOTAGameState->TotalNativePopulation->Population.Size;
	int32 TotalPop = ColonialPop + NativePop;

	if (ColonialPop == 0)
	{
		GOTAGameState->EndGame(GameEnding::Victory, FString("Victory! :)"));
		return;
	}
	if (NativePop == 0)
	{
		GOTAGameState->EndGame(GameEnding::Defeat, FString("Defeat! :("));
		return;
	}

	// based on Culture
	int32 TotalColonistFollower = GOTAGameState->TotalColonialPopulation->Population.FollowerColonists
		+ GOTAGameState->TotalNativePopulation->Population.FollowerColonists;
	int32 TotalNativeFollower = GOTAGameState->TotalColonialPopulation->GetNativeFollowers()
		+ GOTAGameState->TotalNativePopulation->GetNativeFollowers();

	if (TotalColonistFollower == 0)
	{
		GOTAGameState->EndGame(GameEnding::Victory, FString("Victory! :)"));
		return;
	}

	if (TotalNativeFollower == 0)
	{
		GOTAGameState->EndGame(GameEnding::Defeat, FString("Defeat! :("));
		return;
	}

	//based on Ecovalues
	float TreeRatio = static_cast<float>(GOTAGameState->IslandMaxTrees) / GOTAGameState->TotalTrees->Current;
	float WildlifeRatio = static_cast<float>(GOTAGameState->IslandMaxWildlife) / GOTAGameState->TotalWildlife->Current;
	float ForageRatio = static_cast<float>(GOTAGameState->IslandMaxForage) / GOTAGameState->TotalForage->Current;
	int32 EcoUnderRatioCount = 0;
	if (TreeRatio < GameBalance->GameEndingEcoThreshold) ++EcoUnderRatioCount;
	if (WildlifeRatio < GameBalance->GameEndingEcoThreshold) ++EcoUnderRatioCount;
	if (ForageRatio < GameBalance->GameEndingEcoThreshold) ++EcoUnderRatioCount;
	if (EcoUnderRatioCount >= 2)
		GOTAGameState->EndGame(GameEnding::Defeat, FString("Defeat! :("));
}
