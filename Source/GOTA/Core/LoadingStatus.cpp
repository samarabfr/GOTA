// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingStatus.h"
#include "Net/UnrealNetwork.h"

void ALoadingStatus::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALoadingStatus, NetworkedReplicationCount);
	DOREPLIFETIME(ALoadingStatus, CurrentStatus);
	DOREPLIFETIME(ALoadingStatus, GOTAPlayerID);
}

void ALoadingStatus::IncreaseReplicationCount()
{
	ReplicationCount++;
}

void ALoadingStatus::OnRep_NetworkedReplicationCount(int32 NewCount)
{
	OnChanged.Broadcast();
}

void ALoadingStatus::OnRep_CurrentStatus(ELoadingStatus NewStatus)
{
	OnChanged.Broadcast();
}

void ALoadingStatus::SetNetworkedReplicationCount_Implementation(const int32 NewCount)
{
	NetworkedReplicationCount = NewCount;
	OnChanged.Broadcast();
}

void ALoadingStatus::SetCurrentStatus_Implementation(const ELoadingStatus NewStatus)
{
	CurrentStatus = NewStatus;
	OnChanged.Broadcast();
}