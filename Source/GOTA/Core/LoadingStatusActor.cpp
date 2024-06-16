// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingStatusActor.h"
#include "Net/UnrealNetwork.h"

void ALoadingStatusActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALoadingStatusActor, NetworkedReplicationCount);
	DOREPLIFETIME(ALoadingStatusActor, CurrentStatus);
	DOREPLIFETIME(ALoadingStatusActor, GOTAPlayerID);
}

ALoadingStatusActor::ALoadingStatusActor()
{
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ALoadingStatusActor::IncreaseReplicationCount()
{
	ReplicationCount++;
}

void ALoadingStatusActor::OnRep_NetworkedReplicationCount(int32 NewCount)
{
	OnChanged.Broadcast();
}

void ALoadingStatusActor::OnRep_CurrentStatus(ELoadingStatus NewStatus)
{
	OnChanged.Broadcast();
}

void ALoadingStatusActor::SetNetworkedReplicationCount_Implementation(const int32 NewCount)
{
	NetworkedReplicationCount = NewCount;
	OnChanged.Broadcast();
}

void ALoadingStatusActor::SetCurrentStatus_Implementation(const ELoadingStatus NewStatus)
{
	CurrentStatus = NewStatus;
	OnChanged.Broadcast();
}