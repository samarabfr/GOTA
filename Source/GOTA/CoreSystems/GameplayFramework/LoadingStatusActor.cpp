// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingStatusActor.h"
#include "Net/UnrealNetwork.h"

void ALoadingStatusActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALoadingStatusActor, NetRepCount);
	DOREPLIFETIME(ALoadingStatusActor, CurrentStatus);
	DOREPLIFETIME(ALoadingStatusActor, GOTAPlayerID);
}

ALoadingStatusActor::ALoadingStatusActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ALoadingStatusActor::IncreaseReplicationCount()
{
	++RepCount;
}

void ALoadingStatusActor::OnRep_NetRepCount(int32 NewCount)
{
	OnChanged.Broadcast();
}

void ALoadingStatusActor::SetNetRepCount_Implementation(const int32 NewCount)
{
	NetRepCount = NewCount;
	OnChanged.Broadcast();
}

void ALoadingStatusActor::OnRep_CurrentStatus(ELoadingStatus NewStatus)
{
	OnChanged.Broadcast();
}

void ALoadingStatusActor::SetCurrentStatus_Implementation(const ELoadingStatus NewStatus)
{
	CurrentStatus = NewStatus;
	OnChanged.Broadcast();
}
