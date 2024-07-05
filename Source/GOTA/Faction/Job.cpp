// Fill out your copyright notice in the Description page of Project Settings.


#include "Job.h"
#include "Net/UnrealNetwork.h"

void UJob::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UJob, IsWorked);
	DOREPLIFETIME(UJob, Tier);
	DOREPLIFETIME(UJob, DataAsset);
}

bool UJob::IsSupportedForNetworking() const
{
	return true;
}

UJob::UJob()
{
	IsWorked = false;
	Tier = 1;
	DataAsset = nullptr;
}

void UJob::Setup(UJobDataAsset* InDataAsset, int32 InTier)
{
	Tier = InTier;
	DataAsset = InDataAsset;
}

FJobIncome UJob::GetJobIncome()
{
	return DataAsset->JobIncomeByTier[Tier-1];
}
