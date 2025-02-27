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
	DOREPLIFETIME_WITH_PARAMS(UResourceStorage, ResourceLimit, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(UResourceStorage, CurrentResources, Params);
}

bool UResourceStorage::IsSupportedForNetworking() const
{
	return true;
}

void UResourceStorage::S_Init(FGameResources ResourceLimit, bool ReplicateOnAdding, bool ReplicateOnRemoving)
{
	S_SetReplicateOnAdding(ReplicateOnAdding);
	S_SetReplicateOnRemoving(ReplicateOnRemoving);
	S_SetResourceLimit(ResourceLimit);
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

void UResourceStorage::S_SetResourceLimit(FGameResources NewResourceLimit)
{
	ResourceLimit = NewResourceLimit;
	MARK_PROPERTY_DIRTY_FROM_NAME(UResourceStorage, ResourceLimit, this)
}

FGameResources UResourceStorage::GetResourceLimit() const
{
	return ResourceLimit;
}

// ------------------------------------ Current Resources --------------------------------------

void UResourceStorage::OnRep_CurrentResources()
{
	OnCurrentResourcesChanged.Broadcast(CurrentResources);
}

void UResourceStorage::AddResources(FGameResources Resources)
{
	CurrentResources.Food = FMath::Min(ResourceLimit.Food,
								   CurrentResources.Food + FMath::Max(0.f, Resources.Food));
	CurrentResources.Wood = FMath::Min(ResourceLimit.Wood,
									   CurrentResources.Wood + FMath::Max(0.f, Resources.Wood));
	CurrentResources.Stone = FMath::Min(ResourceLimit.Stone,
										CurrentResources.Stone + FMath::Max(0.f, Resources.Stone));
}

void UResourceStorage::RemoveResources(FGameResources Resources)
{
	CurrentResources.Food = FMath::Max(0.f, CurrentResources.Food - FMath::Max(0.f, Resources.Food));
	CurrentResources.Wood = FMath::Max(0.f, CurrentResources.Wood - FMath::Max(0.f, Resources.Wood));
	CurrentResources.Stone = FMath::Max(0.f, CurrentResources.Stone - FMath::Max(0.f, Resources.Stone));
}

FGameResources UResourceStorage::GetCurrentResources() const
{
	return CurrentResources;
}

bool UResourceStorage::IsEmpty()
{
	return CurrentResources == FGameResources::Zero();
}

bool UResourceStorage::IsFull()
{
	return CurrentResources == ResourceLimit;
}

void UResourceStorage::S_AddResources(FGameResources Resources)
{
	const FGameResources OldResources = CurrentResources;
	AddResources(Resources);
	// Events and replication
	if (OldResources != CurrentResources)
	{
		OnCurrentResourcesChanged.Broadcast(CurrentResources);
		if (IsFull())
		{
			OnIsFullChanged.Broadcast(true);
		}
		if (!IsEmpty())
		{
			OnIsEmptyChanged.Broadcast(false);
		}
		if (bReplicateOnAdding)
		{
			MARK_PROPERTY_DIRTY_FROM_NAME(UResourceStorage, CurrentResources, this)
		}
	}
}

void UResourceStorage::S_RemoveResources(FGameResources Resources)
{
	const FGameResources OldResources = CurrentResources;
	RemoveResources(Resources);
	if (OldResources != CurrentResources)
	{
		OnCurrentResourcesChanged.Broadcast(CurrentResources);
		if (bReplicateOnRemoving)
		{
			MARK_PROPERTY_DIRTY_FROM_NAME(UResourceStorage, CurrentResources, this)
		}
	}
}

void UResourceStorage::C_AddResources(FGameResources Resources)
{
	const FGameResources OldResources = CurrentResources;
	AddResources(Resources);
	if (OldResources != CurrentResources)
	{
		OnCurrentResourcesChanged.Broadcast(CurrentResources);
	}
}

void UResourceStorage::C_RemoveResources(FGameResources Resources)
{
	const FGameResources OldResources = CurrentResources;
	RemoveResources(Resources);
	if (OldResources != CurrentResources)
	{
		OnCurrentResourcesChanged.Broadcast(CurrentResources);
	}
}
