// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingProduction.h"
#include "Enums.h"
#include "BuildingTierData.h"
#include "Net/UnrealNetwork.h"

void UBuildingProduction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBuildingProduction, PopulationThreshold);
	DOREPLIFETIME(UBuildingProduction, ProductionPerThreshold);
	DOREPLIFETIME(UBuildingProduction, Production);
	DOREPLIFETIME(UBuildingProduction, ProductionType);
}

bool UBuildingProduction::IsSupportedForNetworking() const
{
	return true;
}

void UBuildingProduction::RecalculateProduction()
{
	if (PopulationThreshold <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("PopulationThreshold is zero or negative, cannot recalculate production."));
		return;
	}
	int32 OldProduction = Production;
	int32 HowOften = CurrentPopulation / PopulationThreshold;
	Production = HowOften * ProductionPerThreshold;
	if (OldProduction != Production)
	{
		OnProductionChanged.Broadcast(Production - OldProduction, ProductionType);
		OnChanged.Broadcast();
	}
}

void UBuildingProduction::BindToPopulation(UPopulation* Population)
{
	CurrentPopulation = Population->Current;
	Population->OnPopulationChanged.AddDynamic(this, &UBuildingProduction::UpdatePopulation);
}

void UBuildingProduction::UpdatePopulation(int32 Change)
{
	if (Change == 0) return;
	CurrentPopulation += Change;
	RecalculateProduction();
	OnChanged.Broadcast();
}

void UBuildingProduction::Setup(int32 NewPopulationThreshold, int32 NewProductionPerThreshold,
                                EProductionType NewProductionType)
{
	PopulationThreshold = NewPopulationThreshold;
	ProductionPerThreshold = NewProductionPerThreshold;
	ProductionType = NewProductionType;
	RecalculateProduction();
}

void UBuildingProduction::SetupWithTierData(FBuildingTierData* TierData)
{
	PopulationThreshold = TierData->PopulationThreshold;
	ProductionPerThreshold = TierData->ProductionPerThreshold;
	ProductionType = TierData->ProductionType;
	RecalculateProduction();
}
