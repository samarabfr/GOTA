// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSummary.h"

#include "GOTA/CoreSystems/Faction/Building/BuildingDataAsset.h"

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
	for (int i = 0; i < static_cast<int32>(EProductionType::Enum_Length); i++)
	{
		ProductionMap.Add(static_cast<EProductionType>(i), 0);
	}
}

void UBuildingSummary::RegisterBuildingProduction(UBuilding* Building)
{
	if (!Building) return;
	Building->OnProductionChanged.AddDynamic(this, &UBuildingSummary::UpdateBuildingProduction);
	UpdateBuildingProduction(Building->GetCurrentProduction(), Building->DataAsset->ProductionType);
}

void UBuildingSummary::UnregisterBuildingProduction(UBuilding* Building)
{
	if (!Building) return;
	Building->OnProductionChanged.RemoveDynamic(this, &UBuildingSummary::UpdateBuildingProduction);
	UpdateBuildingProduction(-Building->GetCurrentProduction(), Building->DataAsset->ProductionType);
}

void UBuildingSummary::UpdateBuildingProduction(float Change, EProductionType Type)
{
	if(Change == 0) return;
	ProductionMap[Type] += Change;
	OnChanged.Broadcast();
}
