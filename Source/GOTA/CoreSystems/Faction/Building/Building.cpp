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
	Population->OnPopulationChanged.AddDynamic(this, &UBuilding::UpdateProduction);
}

bool UBuilding::CanUpgrade()
{
	FBuildingTierData* NewTierData = DataAsset->GetTierData(Tier + 1);
	return NewTierData && NewTierData->Housing > 0;
}

void UBuilding::Upgrade()
{
	FBuildingTierData* NewTierData = DataAsset->GetTierData(Tier + 1);
	SetupProduction(NewTierData);
	int32 EC = 0;
	Population->ChangeMaximum(NewTierData->Housing - Population->Maximum, EC);
}

void UBuilding::UpdateProduction(int32 Change)
{
	int32 OldProduction = Production;
	Production = ProductionPerThreshold * (Population->Current / PopulationThreshold);
	if (OldProduction != Production)
	{
		OnProductionChanged.Broadcast(Production - OldProduction, ProductionType);
	}
}

void UBuilding::SetupProduction(const FBuildingTierData* TierData)
{
	PopulationThreshold = TierData->PopulationThreshold;
	ProductionPerThreshold = TierData->ProductionPerThreshold;
	ProductionType = TierData->ProductionType;
	// can't use UpdateProduction() because it should always call the delegate in this case
	int32 OldProduction = Production;
	Production = ProductionPerThreshold * (Population->Current / PopulationThreshold);
	OnProductionChanged.Broadcast(Production - OldProduction, ProductionType);
}
