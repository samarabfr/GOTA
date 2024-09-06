// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"
#include "BuildingDataAsset.h"
#include "Net/UnrealNetwork.h"

void UBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBuilding, PopContainer);
	DOREPLIFETIME(UBuilding, Production);
	DOREPLIFETIME(UBuilding, Tier);
	DOREPLIFETIME(UBuilding, DataAsset);
}

bool UBuilding::IsSupportedForNetworking() const
{
	return true;
}

UBuilding::UBuilding()
{
	PopContainer = CreateDefaultSubobject<UPopulation>(TEXT("Population"));
//	PopContainer->OnPopulationChanged.AddDynamic(this, &UBuilding::UpdateProduction);
}

bool UBuilding::CanUpgrade()
{
	FBuildingTierData* NewTierData = DataAsset->GetTierData(Tier + 1);
	return NewTierData && NewTierData->Housing > 0;
}

void UBuilding::Upgrade()
{
	FBuildingTierData* NewTierData = DataAsset->GetTierData(Tier + 1);
//	SetupProduction(NewTierData);
//	PopContainer->ChangeMaxSize(NewTierData->Housing - PopContainer->Population.MaxSize);
	++Tier;
}

