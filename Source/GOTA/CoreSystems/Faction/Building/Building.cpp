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
	PopContainer = CreateDefaultSubobject<UPopulationContainer>(TEXT("Population"));
	PopContainer->OnPopulationChanged.AddDynamic(this, &UBuilding::UpdateProduction);
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
	PopContainer->ChangeMaxSize(NewTierData->Housing - PopContainer->Population.MaxSize);
	++Tier;
}

void UBuilding::UpdateProduction(FPopulation Change)
{
	if(PopulationThreshold <= 0) return;
	int32 OldProduction = Production;
	Production = ProductionPerThreshold * (PopContainer->Population.Size / PopulationThreshold);
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
	Production = ProductionPerThreshold * (PopContainer->Population.Size / PopulationThreshold);
	OnProductionChanged.Broadcast(Production - OldProduction, ProductionType);
}

UCombatValues* UBuilding::GetCombatValues()
{
	return PopContainer->CombatValues;
}
