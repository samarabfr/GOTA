// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingStatusActor.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

void ALoadingStatusActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALoadingStatusActor, NetRepCount);
	DOREPLIFETIME(ALoadingStatusActor, GOTAPlayerID);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_SkipOwner;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS_FAST(ALoadingStatusActor, CurrentStatus, Params);
}

ALoadingStatusActor::ALoadingStatusActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ALoadingStatusActor::Delete()
{
	if (HasAuthority())
	{
		Destroy();
	}
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

void ALoadingStatusActor::SetCurrentStatus(const ELoadingStatus NewStatus)
{
	if(!HasAuthority())
	{
		CurrentStatus = NewStatus;
		OnChanged.Broadcast();
	}
	SetCurrentStatusServer(NewStatus);
}

void ALoadingStatusActor::SetCurrentStatusServer_Implementation(const ELoadingStatus NewStatus)
{
	CurrentStatus = NewStatus;
	MARK_PROPERTY_DIRTY_FROM_NAME(ALoadingStatusActor, CurrentStatus, this);
	OnChanged.Broadcast();
}
