// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

#include "BuildingSettings.h"
#include "Population.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// ------------------------------------ Replication Setup --------------------------------------

void UBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(UBuilding, Settings, Params);
	DOREPLIFETIME_WITH_PARAMS(UBuilding, Tile, Params);
	DOREPLIFETIME_WITH_PARAMS(UBuilding, Settlement, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(UBuilding, Population, Params);
	DOREPLIFETIME_WITH_PARAMS(UBuilding, bIsUnderConstruction, Params);
	DOREPLIFETIME_WITH_PARAMS(UBuilding, ConstructionProgress, Params);
}

bool UBuilding::IsSupportedForNetworking() const
{
	return true;
}

// ---------------------------------------- Lifecycle ----------------------------------------

UBuilding::UBuilding()
{
	Population = CreateDefaultSubobject<UPopulation>(TEXT("Population"));
	Population->OnSizeChanged.AddDynamic(this, &UBuilding::PopulationChanged);
}

void UBuilding::S_Init(UBuildingSettings* InSettings, ATile* InTile, ASettlement* InSettlement)
{
	Population->S_Init(InSettlement->GetGrowthPerOwnPop(), InSettlement->GetGrowthPerNeighborPop());
	Settings = InSettings;
	Tile = InTile;
	S_SetSettlement(InSettlement);
	bIsUnderConstruction = true;
}

void UBuilding::C_Init()
{
}

void UBuilding::S_Tick(const float DeltaSeconds)
{
	Population->S_Tick(DeltaSeconds);
}

void UBuilding::C_Tick(const float DeltaSeconds)
{
	Population->C_Tick(DeltaSeconds);
}

void UBuilding::S_PrepareDestroy()
{
	if (Settlement) Settlement->UnregisterPopulation(Population);
}

void UBuilding::C_PrepareDestroy()
{
	if (Settlement) Settlement->UnregisterPopulation(Population);
}

// ---------------------------------------- Utility ----------------------------------------

// --------------------------------------- Settlement ---------------------------------------

void UBuilding::OnRep_Settlement()
{
	if (Settlement) Settlement->RegisterPopulation(Population);
}

void UBuilding::S_SetSettlement(ASettlement* InSettlement)
{
	Settlement = InSettlement;
	if (Settlement) Settlement->RegisterPopulation(Population);
	MARK_PROPERTY_DIRTY_FROM_NAME(UBuilding, Settlement, this)
}

// --------------------------------------- Population ---------------------------------------

void UBuilding::PopulationChanged(int16 Change)
{
	RefreshEfficiency();
}

// --------------------------------------- Efficiency ---------------------------------------

void UBuilding::SetEfficiency(const float NewEfficiency)
{
	const float Change = NewEfficiency - Efficiency;
	if (FMath::IsNearlyZero(Change)) return;

	Efficiency = NewEfficiency;
	OnEfficiencyChanged.Broadcast(Change);

	// when Efficiency changes, the predicted Production also changes
	OnPredictedProductionChanged.Broadcast(Settings->GetDefaultPredictedProduction() * Change,
	                                       Settings->ProductionType);
	OnPredictedConsumptionChanged.Broadcast(Settings->GetDefaultPredictedConsumption() * Change,
	                                        Settings->ConsumptionType);
}

void UBuilding::RefreshEfficiency()
{
	SetEfficiency(Population->GetSize() / static_cast<float>(Settings->Housing));
}

// ------------------------------------- Predicted Production ---------------------------------------

EProductionType UBuilding::GetProductionType() const
{
	if (Settings)
		return Settings->ProductionType;
	return EProductionType::None;
}

float UBuilding::GetPredictedProduction() const
{
	if (GetProductionType() != EProductionType::None)
		return Settings->GetDefaultPredictedProduction() * Efficiency;
	return 0.0f;
}

EConsumptionType UBuilding::GetConsumptionType() const
{
	if (Settings)
		return Settings->ConsumptionType;
	return EConsumptionType::None;
}

float UBuilding::GetPredictedConsumption() const
{
	if (GetConsumptionType() != EConsumptionType::None)
		return Settings->GetDefaultPredictedConsumption() * Efficiency;
	return 0.0f;
}

// --------------------- Construction phase ---------------------

FGameResources UBuilding::GetConstructionProgress() const
{
	return ConstructionProgress;
}

void UBuilding::SetConstructionProgress(const FGameResources NewConstructionProgress)
{
	ConstructionProgress = NewConstructionProgress;
	MARK_PROPERTY_DIRTY_FROM_NAME(UBuilding, ConstructionProgress, this)
	if (ConstructionProgress >= Settings->Cost)
		FinishConstruction();
}

void UBuilding::FinishConstruction()
{
	bIsUnderConstruction = false;
	MARK_PROPERTY_DIRTY_FROM_NAME(UBuilding, bIsUnderConstruction, this)
	Tile->OnBuildingFinishedConstruction();
	Population->S_ChangeMaxSize(Settings->Housing);
}

// --------------------- Protection ---------------------

bool UBuilding::IsProtected() const
{
	for (ATile* ClaimedTile : Settlement->ClaimedTiles)
	{
		if (ClaimedTile &&
			ClaimedTile->GetBuilding() &&
			ClaimedTile->GetBuilding()->Settings->bDefenseEnabled)
		{
			const int32 TileDistance = Tile->GetPathTileDistanceTo(ClaimedTile);
			if (TileDistance > 0 && ClaimedTile->GetBuilding()->Settings->RavageProtectionRange >= TileDistance)
			{
				return true;
			}
		}
	}
	return false;
}
