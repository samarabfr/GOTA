// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceStorage.h"

#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

void UResourceStorage::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(UResourceStorage, bReplicateOnAdding, Params);
	DOREPLIFETIME_WITH_PARAMS(UResourceStorage, bReplicateOnRemoving, Params);
	DOREPLIFETIME_WITH_PARAMS(UResourceStorage, Limit, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(UResourceStorage, Current, Params);
}

bool UResourceStorage::IsSupportedForNetworking() const
{
	return true;
}

void UResourceStorage::S_Init(float InLimit, bool ReplicateOnAdding, bool ReplicateOnRemoving)
{
	S_SetReplicateOnAdding(ReplicateOnAdding);
	S_SetReplicateOnRemoving(ReplicateOnRemoving);
	S_SetLimit(InLimit);
}

// ------------------------------------ Utility --------------------------------------

void UResourceStorage::S_SetReplicateOnAdding(bool ReplicateOnAdding)
{
	bReplicateOnAdding = ReplicateOnAdding;
	MARK_PROPERTY_DIRTY_FROM_NAME(UResourceStorage, bReplicateOnAdding, this)
}

void UResourceStorage::S_SetReplicateOnRemoving(bool ReplicateOnRemoving)
{
	bReplicateOnRemoving = ReplicateOnRemoving;
	MARK_PROPERTY_DIRTY_FROM_NAME(UResourceStorage, bReplicateOnRemoving, this)
}

void UResourceStorage::S_SetLimit(float NewLimit)
{
	Limit = NewLimit;
	MARK_PROPERTY_DIRTY_FROM_NAME(UResourceStorage, Limit, this)
}

float UResourceStorage::GetLimit() const
{
	return Limit;
}

// ------------------------------------ Current Resources --------------------------------------

void UResourceStorage::OnRep_Current()
{
	OnCurrentChanged.Broadcast(Current);
}

void UResourceStorage::SetCurrent(const float NewCurrent)
{
	if (NewCurrent != Current)
	{
		const bool bWasFull = IsFull();
		const bool bWasEmpty = IsEmpty();
		Current = NewCurrent;
		OnCurrentChanged.Broadcast(Current);
		if (bWasFull != IsFull())
		{
			OnIsFullChanged.Broadcast(IsFull());
		}
		if (bWasEmpty != IsEmpty())
		{
			OnIsEmptyChanged.Broadcast(IsEmpty());
		}
	}
}

float UResourceStorage::GetCurrent() const
{
	return Current;
}

bool UResourceStorage::IsEmpty() const
{
	return Current == 0.0f;
}

bool UResourceStorage::IsFull() const
{
	return Current == Limit;
}

void UResourceStorage::Add(float Amount)
{
	SetCurrent(FMath::Min(Limit, Current + FMath::Max(0.f, Amount)));
}

void UResourceStorage::Remove(float Amount)
{
	SetCurrent(FMath::Max(0.f, Current - FMath::Max(0.f, Amount)));
}

void UResourceStorage::Empty()
{
	SetCurrent(0.0f);
}

void UResourceStorage::Fill()
{
	
	SetCurrent(Limit);
}

void UResourceStorage::S_Add(float Amount)
{
	const float Old = Current;
	Add(Amount);
	// Events and replication
	if (Old != Current && bReplicateOnAdding)
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(UResourceStorage, Current, this)
	}
}

void UResourceStorage::S_Remove(float Amount)
{
	const float Old = Current;
	Remove(Amount);
	if (Old != Current && bReplicateOnRemoving)
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(UResourceStorage, Current, this)
	}
}

void UResourceStorage::S_Empty()
{
	const float Old = Current;
	Empty();
	if (Old != Current && bReplicateOnRemoving)
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(UResourceStorage, Current, this)
	}
}

void UResourceStorage::S_Fill()
{
	const float Old = Current;
	Fill();
	if (Old != Current && bReplicateOnRemoving)
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(UResourceStorage, Current, this)
	}
}
