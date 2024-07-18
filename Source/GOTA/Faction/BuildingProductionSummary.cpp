// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingProductionSummary.h"

UBuildingProductionSummary::UBuildingProductionSummary()
{
	for (int i = 0; i < static_cast<int32>(EProductionType::MAX); i++)
	{
		ProductionMap.Add(static_cast<EProductionType>(i), 0);
	}
}

void UBuildingProductionSummary::RegisterBuildingProduction(UBuildingProduction* BuildingProduction)
{
	if (!BuildingProduction)
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't register Building Production, nullptr"));
	}
	BuildingProduction->OnProductionChanged.AddDynamic(this, &UBuildingProductionSummary:: UpdateBuildingProduction);
	if(BuildingProduction->Production != 0)
	{
		ProductionMap[BuildingProduction->ProductionType] += BuildingProduction->Production;
		OnChanged.Broadcast();
	}
}

void UBuildingProductionSummary::UpdateBuildingProduction(int32 Change, EProductionType Type)
{
	if(Change == 0) return;
	ProductionMap[Type] += Change;
	OnChanged.Broadcast();
}
