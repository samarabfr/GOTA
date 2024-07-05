// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"
#include "Net/UnrealNetwork.h"

void UBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBuilding, Jobs);
}

bool UBuilding::IsSupportedForNetworking() const
{
	return true;
}

const TArray<UJob*>& UBuilding::GetJobs() const
{
	return Jobs;
}

void UBuilding::AddJob(UJob* Job)
{
	Jobs.Add(Job);
	OnJobAdded.Broadcast(Job);
}

void UBuilding::RemoveJob(UJob* Job)
{
	Jobs.Remove(Job);
	OnJobRemoved.Broadcast(Job);
}
