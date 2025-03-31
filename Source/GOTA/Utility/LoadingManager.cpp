// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingManager.h"

#include "DaytimeManager.h"
#include "LoadingStatusActor.h"
#include "GameFramework/GameUserSettings.h"
#include "GOTA/AI/SettlementAIs/ColonyAI_T1.h"
#include "GOTA/GameplayFramework/GM_Ingame.h"
#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/GameplayFramework/PC_Ingame.h"
#include "GOTA/GameplayFramework/PS_Ingame.h"
#include "GOTA/Settlement/Settlement.h"
#include "GOTA/Tilemap/TileMap.h"
#include "Net/UnrealNetwork.h"

void ALoadingManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALoadingManager, LoadingStatuses);
}

ALoadingManager::ALoadingManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ALoadingManager::Delete()
{
	for (ALoadingStatusActor* Status : LoadingStatuses)
	{
		if (Status) Status->Delete();
	}
	if (HasAuthority())
	{
		Destroy();
	}
}

void ALoadingManager::BeginPlay()
{
	Super::BeginPlay();
	GameMode = GetWorld()->GetAuthGameMode<AGM_Ingame>();
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
	GameState->SetLoadingManager(this);
	LocalPlayerController = GetWorld()->GetFirstPlayerController<APC_Ingame>();
	if(LocalPlayerController)
	{
		LocalPlayerController->RemoveLobbyUI();
		LocalPlayerController->CreateLoadingUI();
	}

	// We have to Apply the GameUserSettings once we are on the Island.lvl, and before we show the game because
	// for some reason some CVars get set when changing the level
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
	UserSettings->LoadSettings();
	UserSettings->ApplySettings(true);
}

void ALoadingManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds); // Ensure this is called to maintain ticking

	if (HasAuthority())
	{
		ServerTick();
	}
	else
	{
		ClientTick();
	}
	GracePeriodTime += DeltaSeconds;
}

void ALoadingManager::ServerTick()
{
	if (!LoadingStatus)
	{
		GOTAPlayerID = 0;
		SpawnLoadingStatuses();
		LoadingStatus = LoadingStatuses[0];
		if(!LoadingStatus) return;
	}
	switch (LoadingStatus->CurrentStatus)
	{
	case ELoadingStatus::NotStarted:
		LoadingStatus->SetCurrentStatus(ELoadingStatus::WaitForReadyForCreation);
		break;

	case ELoadingStatus::WaitForReadyForCreation:
		if (IsEveryoneOn(ELoadingStatus::WaitForReadyForCreation))
		{
			GameMode->CreateWorld();
			GameMode->CreateSettlements();
			GameMode->CreateGuardians();
			GameMode->InitPlayerControllers();
			GameState->CountIslandMaxEcoValues();
			LoadingStatus->SetNetRepCount(LoadingStatus->RepCount);
			LoadingStatus->SetCurrentStatus(ELoadingStatus::WaitForReplication);
		}
		break;

	case ELoadingStatus::WaitForReplication:
		if (IsEveryoneOn(ELoadingStatus::WaitForReplication))
		{
			LocalPlayerController->CreateIngameUI();
			GameMode->InitialPossession();
			LoadingStatus->SetCurrentStatus(ELoadingStatus::WaitForFinished);
			GameState->GetTileMap()->MaxAllEcoValues();
			GracePeriodTime = 0.0;
		}
		break;

	case ELoadingStatus::WaitForFinished:
		if (IsEveryoneOn(ELoadingStatus::WaitForFinished) && GracePeriodTime > 2)
		{
			LocalPlayerController->InitInput();
			LocalPlayerController->RemoveLoadingUI();
			GameMode->StartGame();
			GameState->GetTileMap()->EnableTick();
			GameState->GetColony()->EnableTick();
			GameState->GetTribe()->EnableTick();
			GameState->DaytimeManager->SetActorTickEnabled(true);
			LoadingStatus->SetCurrentStatus(ELoadingStatus::Finished);
			GracePeriodTime = 0.0;
		}
		break;

	case ELoadingStatus::Finished:
		if (GracePeriodTime > 2)
		{
			for (ALoadingStatusActor* LoadingStatusActor : LoadingStatuses)
			{
				if (LoadingStatusActor) LoadingStatusActor->Destroy();
			}
			Destroy();
		}
		break;

	default:
		break;
	}
}

void ALoadingManager::ClientTick()
{
	// Check for GOTAPlayerID
	if (GOTAPlayerID < 0 && LocalPlayerController && LocalPlayerController->GetPlayerState<APS_Ingame>())
		GOTAPlayerID = LocalPlayerController->GetPlayerState<APS_Ingame>()->GOTAPlayerID;
	if (GOTAPlayerID < 0)
		return;
	// Check for Server LoadingStatus
	if (!LoadingStatuses[0]) return;
	// Check for LoadingStatus
	if (!LoadingStatus)
	{
		if (LoadingStatuses[GOTAPlayerID])
			LoadingStatus = LoadingStatuses[GOTAPlayerID];
		else
			return;
	}

	switch (LoadingStatus->CurrentStatus)
	{
	case ELoadingStatus::NotStarted:
		LoadingStatus->SetCurrentStatus(ELoadingStatus::WaitForReadyForCreation);
		break;

	case ELoadingStatus::WaitForReadyForCreation:
		if (LoadingStatuses[0]->CurrentStatus == ELoadingStatus::WaitForReplication)
			LoadingStatus->SetCurrentStatus(ELoadingStatus::Replicating);
		break;

	case ELoadingStatus::Replicating:
		if (LoadingStatuses[0]->NetRepCount > 0
			&& LoadingStatuses[0]->NetRepCount == LoadingStatus->NetRepCount)
			LoadingStatus->SetCurrentStatus(ELoadingStatus::WaitForReplication);
		LoadingStatus->SetNetRepCount(LoadingStatus->RepCount);
		break;

	case ELoadingStatus::WaitForReplication:
		if (LoadingStatuses[0]->CurrentStatus == ELoadingStatus::WaitForFinished)
		{
			LocalPlayerController->CreateIngameUI();
			LoadingStatus->SetCurrentStatus(ELoadingStatus::WaitForFinished);
		}
		break;

	case ELoadingStatus::WaitForFinished:
		if (LoadingStatuses[0]->CurrentStatus == ELoadingStatus::Finished)
		{
			LocalPlayerController->InitInput();
			LocalPlayerController->RemoveLoadingUI();
			GameState->GetTileMap()->EnableTick();
			GameState->DaytimeManager->SetActorTickEnabled(true);
			LoadingStatus->SetCurrentStatus(ELoadingStatus::Finished);
		}
		break;

	default:
		break;
	}
}

void ALoadingManager::SpawnLoadingStatuses()
{
	LoadingStatuses.SetNumZeroed(4);
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		APS_Ingame* PS = Cast<APS_Ingame>(PlayerState);
		if(!PS || PS->GOTAPlayerID < 0) return;
		ALoadingStatusActor* LSA = GetWorld()->SpawnActor<ALoadingStatusActor>();
		LSA->SetOwner(PS->GetOwningController());
		LSA->GOTAPlayerID = PS->GOTAPlayerID;
		LoadingStatuses[PS->GOTAPlayerID] = LSA;
	}
}

bool ALoadingManager::IsEveryoneOn(ELoadingStatus Status)
{
	for (ALoadingStatusActor* LS : LoadingStatuses)
	{
		if (LS && LS->CurrentStatus != Status) return false;
	}
	return true;
}


void ALoadingManager::IncrementReplicationCount()
{
	LoadingStatuses[GOTAPlayerID]->IncreaseReplicationCount();
}
