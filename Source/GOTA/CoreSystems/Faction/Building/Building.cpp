// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

#include "BuildingDataAsset.h"
#include "Net/UnrealNetwork.h"

void UBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBuilding, Population);
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
	Population = CreateDefaultSubobject<UPopulation>(TEXT("Population"));
	Production = CreateDefaultSubobject<UBuildingProduction>(TEXT("Production"));
	Production->BindToPopulation(Population);
}

bool UBuilding::Upgrade()
{
	FBuildingTierData* NewTierData = DataAsset->GetTierData(Tier + 1);
	if (NewTierData->Housing < 0) return false;
	Production->SetupWithTierData(NewTierData);
	int32 EC = 0;
	Population->ChangeMaximum(NewTierData->Housing - Population->Maximum, EC);
	return true;
}
