// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSummary.h"

void UBuildingSummary::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool UBuildingSummary::IsSupportedForNetworking() const
{
	return true;
}

UBuildingSummary::UBuildingSummary()
{
	for (int i = 0; i < static_cast<int32>(EProductionType::MAX); i++)
	{
		ProductionMap.Add(static_cast<EProductionType>(i), 0);
	}
}

void UBuildingSummary::RegisterBuildingProduction(UBuilding* Building)
{
	if (!Building) return;
	Building->OnProductionChanged.AddDynamic(this, &UBuildingSummary::UpdateBuildingProduction);
	UpdateBuildingProduction(Building->Production, Building->ProductionType);
}

void UBuildingSummary::UnregisterBuildingProduction(UBuilding* Building)
{
	if (!Building) return;
	Building->OnProductionChanged.RemoveDynamic(this, &UBuildingSummary::UpdateBuildingProduction);
	UpdateBuildingProduction(-Building->Production, Building->ProductionType);
}

void UBuildingSummary::UpdateBuildingProduction(int32 Change, EProductionType Type)
{
	if(Change == 0) return;
	ProductionMap[Type] += Change;
	OnChanged.Broadcast();
}
