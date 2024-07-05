// Fill out your copyright notice in the Description page of Project Settings.


#include "JobManager.h"

#include "Building.h"
#include "Algo/RandomShuffle.h"
#include "Net/UnrealNetwork.h"

void UJobManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UJobManager, AvailableWorkforce);
	DOREPLIFETIME(UJobManager, CurrentlyEmployed);
	DOREPLIFETIME(UJobManager, Jobs);
	DOREPLIFETIME(UJobManager, CurrentJobIncome);
}

bool UJobManager::IsSupportedForNetworking() const
{
	return true;
}

UJobManager::UJobManager()
{
	
}

void UJobManager::BindToPopulationAttribute(UGOTAAttributePopulation* PopulationAttribute)
{
	PopulationAttribute->OnWorkforceChanged.AddDynamic(this, &UJobManager::WorkForceChanged);
}

void UJobManager::BindToBuilding(UBuilding* Building)
{
	Building->OnJobAdded.AddDynamic(this, &UJobManager::BuildingJobAdded);
	Building->OnJobRemoved.AddDynamic(this, &UJobManager::BuildingJobRemoved);
	for(UJob* Job : Building->GetJobs())
	{
		Jobs.Add(Job);
	}
	DistributeWorkers();
}

void UJobManager::UnbindToBuilding(UBuilding* Building)
{
	Building->OnJobAdded.RemoveDynamic(this, &UJobManager::BuildingJobAdded);
	Building->OnJobRemoved.RemoveDynamic(this, &UJobManager::BuildingJobRemoved);
	for(UJob* Job : Building->GetJobs())
	{
		Jobs.Remove(Job);
	}
	DistributeWorkers();
}

void UJobManager::BuildingJobAdded(UJob* Job)
{
	Jobs.Add(Job);
	DistributeWorkers();
}

void UJobManager::BuildingJobRemoved(UJob* Job)
{
	Jobs.Remove(Job);
	DistributeWorkers();
}

void UJobManager::WorkForceChanged(int32 Change)
{
	AvailableWorkforce += Change;
	DistributeWorkers();
}

void UJobManager::DistributeWorkers()
{
	Algo::RandomShuffle(Jobs);
	CurrentlyEmployed = 0;
	CurrentJobIncome.SetEverythingToZero();
	for(UJob* Job : Jobs)
	{
		if(CurrentlyEmployed < AvailableWorkforce)
		{
			Job->IsWorked = true;
			CurrentlyEmployed++;
			CurrentJobIncome += Job->GetJobIncome();
		} else
		{
			Job->IsWorked = false;
		}
	}
}
