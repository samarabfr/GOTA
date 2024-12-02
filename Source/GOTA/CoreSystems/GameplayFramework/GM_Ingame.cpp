// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Ingame.h"

#include "GameSettings.h"
#include "GOTAGameInstance.h"
#include "LoadingManager.h"
#include "PC_Ingame.h"
#include "PS_Ingame.h"
#include "StartParameter.h"
#include "GameFramework/GameStateBase.h"
#include "GOTA/CoreSystems/Faction/Attribute/GOTAAttribute.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingPlacer.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Faction/Settlement/Colony.h"
#include "GOTA/CoreSystems/Faction/Settlement/SettlementPopulation.h"
#include "GOTA/CoreSystems/Faction/Settlement/Tribe.h"
#include "GOTA/CoreSystems/Guardian/Guardian.h"
#include "GOTA/CoreSystems/Guardian/GuardianSettings.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"
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
	GOTAGameState->S_EndGame(Ending, EndingMessage);
}

void AGM_Ingame::CheckGameEndingConditions()
{
	if (GOTAGameState->GameEnded) return;
	if (!GOTAGameState->GetColony()) return;
	if (!GOTAGameState->GetTribe()) return;

	// based on SettlementPop
	int32 ColonialPop = GOTAGameState->GetColony()->GetPopulation()->GetSize();
	int32 NativePop = GOTAGameState->GetTribe()->GetPopulation()->GetSize();
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
	float ForageRatio = static_cast<float>(GOTAGameState->IslandMaxForage) / GOTAGameState->TotalForage->Current;
	int32 EcoUnderRatioCount = 0;
	if (TreeRatio < GameBalance->GameEndingEcoThreshold) ++EcoUnderRatioCount;
	if (ForageRatio < GameBalance->GameEndingEcoThreshold) ++EcoUnderRatioCount;
	if (EcoUnderRatioCount >= 2)
		EndGame(EGameEnding::Defeat, FString("Defeat! :("));
}

// ---------------------------------------------------------
// World Setup

void AGM_Ingame::CreateWorld()
{
	GOTAGameState->SetTileMap(GetWorld()->SpawnActor<ATileMap>(TileMapClass));
	UGOTAGameInstance* GameInstance = GetGameInstance<UGOTAGameInstance>();
	UWorldGenerator* WorldGen = NewObject<UWorldGenerator>();
	WorldGen->Init(GOTAGameState->GetTileMap(),
	               GOTAGameState->StartParameter->GetIslandSize());
	WorldGen->GenerateWorld();
	GOTAGameState->GetTileMap()->Init();
}

void AGM_Ingame::CreateSettlements()
{
	AGameSettings* GameSettings = GOTAGameState->GetGameSettings();

	AColony* Colony = GetWorld()->SpawnActor<AColony>();
	Colony->S_Init(GOTAGameState->GetTileMap()->GetColonistsStart().Get(),
	               GameSettings->GetColonySettings(),
	               GameSettings->GetColonyPopulationSettings());
	GOTAGameState->SetColony(Colony);

	ATribe* Tribe = GetWorld()->SpawnActor<ATribe>();
	Tribe->S_Init(GOTAGameState->GetTileMap()->GetNativesStart().Get(),
	              GameSettings->GetTribeSettings(),
	              GameSettings->GetTribePopulationSettings());
	GOTAGameState->SetTribe(Tribe);
}

void AGM_Ingame::CreateGuardians()
{
	ATile* TribeStartingTile = GOTAGameState->GetTileMap()->GetNativesStart().Get();
	for (int32 i = 0; i < GOTAGameState->PlayerArray.Num(); ++i)
	{
		const APS_Ingame* PlayerStateIngame = Cast<APS_Ingame>(GOTAGameState->PlayerArray[i]);
		FVector SpawnLocation = FVector(0, 0, 1000);
		if (TribeStartingTile->Neighbors[i])
		{
			SpawnLocation = TribeStartingTile->Neighbors[i]->GetActorLocation() + FVector(0, 0, 100);
		}
		else
		{
			SpawnLocation = TribeStartingTile->GetActorLocation() + FVector(0, 0, 100);
		}
		AGuardian* Guardian = GetWorld()->SpawnActor<AGuardian>(PlayerStateIngame->SelectedGuardian->GuardianBlueprint,
		                                                        SpawnLocation, FRotator::ZeroRotator);
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
		FActorSpawnParameters MouseUtilsSpawnParams;
		MouseUtilsSpawnParams.Owner = PlayerController;
		AMouseUtils* MouseUtils = GetWorld()->SpawnActor<AMouseUtils>(MouseUtilsClass, MouseUtilsSpawnParams);
		PlayerController->SetMouseUtils(MouseUtils);

		// Create BuildingPlacer
		FActorSpawnParameters BuildingPlacerSpawnParams;
		BuildingPlacerSpawnParams.Owner = PlayerController;
		ABuildingPlacer* BuildingPlacer = GetWorld()->SpawnActor<ABuildingPlacer>(BuildingPlacerSpawnParams);
		BuildingPlacer->S_Init(MouseUtils);
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
