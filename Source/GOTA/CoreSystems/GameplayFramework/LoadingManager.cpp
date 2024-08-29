// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingManager.h"

#include "PC_Ingame.h"
#include "PS_Ingame.h"
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
	PrimaryActorTick.TickInterval = 0.2f;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ALoadingManager::BeginPlay()
{
	Super::BeginPlay();
	GameMode = GetWorld()->GetAuthGameMode<AGM_Ingame>();
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
	GameState->LoadingManager = this;
	LocalPlayerController = GetWorld()->GetFirstPlayerController<APC_Ingame>();
	LocalPlayerController->RemoveLobbyUI();
	LocalPlayerController->CreateLoadingUI();
}

void ALoadingManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);  // Ensure this is called to maintain ticking

	if (HasAuthority())
	{
		ServerTick();
	}
	else
	{
		ClientTick();
	}
}

void ALoadingManager::ServerTick()
{
	if (!LoadingStatus)
	{
		GOTAPlayerID = 0;
		SpawnLoadingStatuses();
		LoadingStatus = LoadingStatuses[0];
	}
}

void ALoadingManager::ClientTick()
{
	// Check for GOTAPlayerID
	if (GOTAPlayerID < 0)
		GOTAPlayerID = GetWorld()->GetFirstPlayerController()->GetPlayerState<APS_Ingame>()->GOTAPlayerID;
	if (GOTAPlayerID < 0)
		return;
	// Check for LoadingStatus
	if (!LoadingStatus)
	{
		if (LoadingStatuses[GOTAPlayerID])
			LoadingStatus = LoadingStatuses[GOTAPlayerID];
		else
			return;
	}
}

void ALoadingManager::SpawnLoadingStatuses()
{
	LoadingStatuses.SetNumZeroed(4);
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		APS_Ingame* PS = Cast<APS_Ingame>(PlayerState);
		ALoadingStatusActor* LSA = GetWorld()->SpawnActor<ALoadingStatusActor>();
		LSA->SetOwner(PS->GetOwningController());
		LSA->GOTAPlayerID = PS->GOTAPlayerID;
		LoadingStatuses[PS->GOTAPlayerID] = LSA;
	}
}


void ALoadingManager::IncrementReplicationCount()
{
	LoadingStatuses[GOTAPlayerID]->IncreaseReplicationCount();
}
