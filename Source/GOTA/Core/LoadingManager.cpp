// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingManager.h"

#include "GOTAPlayerController.h"
#include "GOTAPlayerState.h"
#include "Net/UnrealNetwork.h"

void ALoadingManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ALoadingManager, CurrentStatus);
	DOREPLIFETIME(ALoadingManager, ReplicationCounts);
}

ALoadingManager::ALoadingManager()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ALoadingManager::IncreaseReplicationCount()
{
	LocalReplicationCount++;
}

void ALoadingManager::Init()
{
	if(HasAuthority())
	{
		ReplicationCounts.SetNum(GameState->PlayerArray.Num());
		CurrentStatus.Init(ELoadingStatus::InitializingGameState,GameState->PlayerArray.Num());
		AGOTAPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<AGOTAPlayerController>();
		AGOTAPlayerState* PlayerState = PlayerController->GetPlayerState<AGOTAPlayerState>();
		GOTAPlayerID = PlayerState->GOTAPlayerID;
	}
}

void ALoadingManager::SetLoadingStatus_Implementation(int32 Index, ELoadingStatus Status)
{
	CurrentStatus[Index] = Status;
}

void ALoadingManager::SetReplicationCountRPC_Implementation(int32 Index, int32 Count)
{
	ReplicationCounts[GOTAPlayerID] = Count;
}
