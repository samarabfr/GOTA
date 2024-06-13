// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingManager.h"

#include "GOTAGameInstance.h"
#include "GOTAPlayerController.h"
#include "GOTAPlayerState.h"
#include "Net/UnrealNetwork.h"

void ALoadingManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALoadingManager, CurrentStatus);
	DOREPLIFETIME(ALoadingManager, ReplicationCounts);
}

void ALoadingManager::IncreaseReplicationCount()
{
	LocalReplicationCount++;
}

void ALoadingManager::Init()
{
	const UGOTAGameInstance* GI = Cast<UGOTAGameInstance>(GetGameInstance());
	ReplicationCounts.SetNum(GI->PlayerCount);
	CurrentStatus.SetNum(GI->PlayerCount);
	const AGOTAPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<AGOTAPlayerController>();
	const AGOTAPlayerState* PlayerState = PlayerController->GetPlayerState<AGOTAPlayerState>();
}

void ALoadingManager::SetLoadingStatus_Implementation(const int32 Index, const ELoadingStatus Status)
{
	CurrentStatus[Index] = Status;
}

void ALoadingManager::SetReplicationCountRPC_Implementation(const int32 Index, const int32 Count)
{
	ReplicationCounts[Index] = Count;
}
