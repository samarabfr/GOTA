// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Ingame.h"
#include "GOTAGameInstance.h"
#include "LoadingManager.h"
#include "PC_Ingame.h"
#include "PS_Ingame.h"
#include "GameFramework/GameStateBase.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Faction/Settlement/Colony.h"
#include "GOTA/CoreSystems/Faction/Settlement/Tribe.h"
#include "GOTA/CoreSystems/Tile/WorldGenerator.h"
#include "Kismet/GameplayStatics.h"

AGM_Ingame::AGM_Ingame()
{
	ConstructorHelpers::FObjectFinder<UGameBalanceDataAsset> DataAssetFinder(
		TEXT("/Game/CoreSystems/GameplayFramework/DA_GameBalance"));
	GameBalance = DataAssetFinder.Object;
}

// ---------------------------------------------------------
// Control the Flow of the Game

void AGM_Ingame::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
                          FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	if (GetNumPlayers() >= 4)
	{
		ErrorMessage = TEXT("Server is full.");
	}
	AGS_Ingame* GS = Cast<AGS_Ingame>(GameState);
	if (GS->GameStatus != EGameStatus::Lobby)
	{
		ErrorMessage = TEXT("Game is already running.");
	}
}

void AGM_Ingame::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	APS_Ingame* PS = Cast<APS_Ingame>(NewPlayer->PlayerState);
	// find free GOTA ID
	bool FoundFreeID = false;
	int32 FreeID = -1;
	while (!FoundFreeID)
	{
		FreeID++;
		FoundFreeID = true;
		for (APlayerState* PlayerState : GameState->PlayerArray)
		{
			APS_Ingame* GOTAPlayerState = Cast<APS_Ingame>(PlayerState);
			if (GOTAPlayerState->GOTAPlayerID == FreeID)
			{
				FoundFreeID = false;
				break;
			}
		}
	}
	PS->GOTAPlayerID = FreeID;
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

void AGM_Ingame::LoadGame()
{
	GOTAGameState = GetGameState<AGS_Ingame>();
	GOTAGameState->GameStatus = EGameStatus::Loading;
	GetWorld()->SpawnActor<ALoadingManager>();
}

void AGM_Ingame::StartGame()
{
	GOTAGameState->GameStatus = EGameStatus::Running;
	GOTAGameState->ShouldTickTurnTime = true;
}

void AGM_Ingame::TogglePause()
{
	if (IsPaused()) UnpauseGame();
	else PauseGame();
}

void AGM_Ingame::PauseGame()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void AGM_Ingame::UnpauseGame()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void AGM_Ingame::EndGame(EGameEnding Ending, const FString& EndingMessage)
{
	GOTAGameState->GameStatus = EGameStatus::Ended;
	GOTAGameState->EndGame(Ending, EndingMessage);
}

void AGM_Ingame::CheckGameEndingConditions()
{
	if (GOTAGameState->GameEnded) return;

	// based on SettlementPop
	int32 ColonialPop = GOTAGameState->TotalColonialPopulation->GetSize();
	int32 NativePop = GOTAGameState->TotalNativePopulation->GetSize();
	int32 TotalPop = ColonialPop + NativePop;

	if (ColonialPop == 0)
	{
		EndGame(EGameEnding::Victory, FString("Victory! :)"));
		return;
	}
	if (NativePop == 0)
	{
		EndGame(EGameEnding::Defeat, FString("Defeat! :("));
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
		EndGame(EGameEnding::Defeat, FString("Defeat! :("));
}

// ---------------------------------------------------------
// World Setup

void AGM_Ingame::CreateWorld()
{
	GOTAGameState->TileMap = GetWorld()->SpawnActor<ATileMap>(TileMapClass);
	UGOTAGameInstance* GameInstance = GetGameInstance<UGOTAGameInstance>();
	UWorldGenerator* WorldGen = NewObject<UWorldGenerator>();
	WorldGen->Init(GOTAGameState->TileMap,
	               GOTAGameState->StartParameter->GetIslandSize(),
	               GOTAGameState->StartParameter->GetColonies(),
	               1);
	WorldGen->GenerateWorld();
	GOTAGameState->TileMap->Init();
}

void AGM_Ingame::CreateSettlements()
{
	for (ATile* Start : GOTAGameState->TileMap->ColonistsStarts)
	{
		AColony* Colony = GetWorld()->SpawnActor<AColony>();
		Colony->StartingSetup(Start);
		GOTAGameState->Colonies.Add(Colony);
	}
	ATribe* Tribe = GetWorld()->SpawnActor<ATribe>();
	Tribe->StartingSetup(GOTAGameState->TileMap->NativesStarts[0]);
	GOTAGameState->Tribe = Tribe;
}

void AGM_Ingame::CreateGuardians()
{
	// 4 because max players, but maybe this should be a constant somewhere
	GOTAGameState->Guardians.SetNumZeroed(4); 
	for (APlayerState* PlayerState : GOTAGameState->PlayerArray)
	{
		APS_Ingame* PS = Cast<APS_Ingame>(PlayerState);
		FVector Location = FVector(0, 0, 1000);
		AGuardian* G = GetWorld()->SpawnActor<AGuardian>(PS->SelectedGuardian->GuardianBlueprint,
		                                                 Location, FRotator::ZeroRotator);
		GOTAGameState->Guardians[PS->GOTAPlayerID] = G;
	}
}

void AGM_Ingame::CreateMouseUtils()
{
	for (APlayerState* PlayerState : GOTAGameState->PlayerArray)
	{
		APC_Ingame* PC = Cast<APC_Ingame>(PlayerState->GetOwningController());
		AMouseUtils* MU = GetWorld()->SpawnActor<AMouseUtils>(MouseUtilsClass);
		MU->SetOwner(PC);
		PC->SetMouseUtils(MU);
	}
}

void AGM_Ingame::InitialPossession()
{
	GetNumPlayers();
	for (APlayerState* PlayerState : GOTAGameState->PlayerArray)
	{
		APS_Ingame* PS = Cast<APS_Ingame>(PlayerState);
		APC_Ingame* PC = Cast<APC_Ingame>(PS->GetOwningController());
		PC->Possess(GOTAGameState->Guardians[PS->GOTAPlayerID]);
	}
}

void AGM_Ingame::CalculateTurn()
{
	GOTAGameState->TurnCalculationStart();
	StartedCalculatingTurn = FDateTime::Now();
	// Combat Phase
	GOTAGameState->CombatSystem->TriggerAllCombats();
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
	// Finished
	FTimespan TimeSpan = FDateTime::Now() - StartedCalculatingTurn;
	UE_LOG(LogTemp, Warning, TEXT("It took %d ms to calculate the %d turn."),
	       TimeSpan.GetFractionMilli(),
	       GOTAGameState->TurnCounter)
	GOTAGameState->IncreaseTurnCounter();
	GOTAGameState->TurnCalculationEnd();
}
