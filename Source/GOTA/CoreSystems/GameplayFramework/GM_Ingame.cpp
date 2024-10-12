// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Ingame.h"
#include "GOTAGameInstance.h"
#include "LoadingManager.h"
#include "PC_Ingame.h"
#include "PS_Ingame.h"
#include "GameFramework/GameStateBase.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingPlacer.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Faction/Settlement/Colony.h"
#include "GOTA/CoreSystems/Faction/Settlement/Tribe.h"
#include "GOTA/CoreSystems/Guardian/GuardianSettings.h"
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
	if (!GOTAGameState->GetColony()) return;
	if (!GOTAGameState->GetTribe()) return;

	// based on SettlementPop
	int32 ColonialPop = GOTAGameState->GetColony()->Population->GetSize();
	int32 NativePop = GOTAGameState->GetTribe()->Population->GetSize();
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
	               1,
	               1);
	WorldGen->GenerateWorld();
	GOTAGameState->TileMap->Init();
}

void AGM_Ingame::CreateSettlements()
{
	AColony* Colony = GetWorld()->SpawnActor<AColony>();
	Colony->StartingSetup(GOTAGameState->TileMap->ColonistsStarts[0]);
	GOTAGameState->SetColony(Colony);

	ATribe* Tribe = GetWorld()->SpawnActor<ATribe>();
	Tribe->StartingSetup(GOTAGameState->TileMap->NativesStarts[0]);
	GOTAGameState->SetTribe(Tribe);
}

void AGM_Ingame::CreateGuardians()
{
	for (APlayerState* PlayerState : GOTAGameState->PlayerArray)
	{
		const APS_Ingame* PlayerStateIngame = Cast<APS_Ingame>(PlayerState);
		const FVector Location = FVector(0, 0, 1000);
		AGuardian* Guardian = GetWorld()->SpawnActor<AGuardian>(PlayerStateIngame->SelectedGuardian->GuardianBlueprint,
		                                                        Location, FRotator::ZeroRotator);
		GOTAGameState->SetGuardian(PlayerStateIngame->GOTAPlayerID, Guardian);
		Guardian->Init(PlayerStateIngame->SelectedGuardian);
	}
}

void AGM_Ingame::CreateUtilActors()
{
	for (const APlayerState* PlayerState : GOTAGameState->PlayerArray)
	{
		APC_Ingame* PlayerController = Cast<APC_Ingame>(PlayerState->GetOwningController());
		// Create MouseUtils
		AMouseUtils* MouseUtils = GetWorld()->SpawnActor<AMouseUtils>(MouseUtilsClass);
		MouseUtils->SetOwner(PlayerController);
		PlayerController->SetMouseUtils(MouseUtils);
		// Create BuildingPlacer
		ABuildingPlacer* BuildingPlacer = GetWorld()->SpawnActor<ABuildingPlacer>();
		BuildingPlacer->S_Init(MouseUtils);
		BuildingPlacer->SetOwner(PlayerController);
		PlayerController->SetBuildingPlacer(BuildingPlacer);
	}
}

void AGM_Ingame::InitialPossession()
{
	GetNumPlayers();
	for (APlayerState* PlayerState : GOTAGameState->PlayerArray)
	{
		APS_Ingame* PS = Cast<APS_Ingame>(PlayerState);
		APC_Ingame* PC = Cast<APC_Ingame>(PS->GetOwningController());
		PC->Possess(GOTAGameState->GetGuardian(PS->GOTAPlayerID));
	}
}
