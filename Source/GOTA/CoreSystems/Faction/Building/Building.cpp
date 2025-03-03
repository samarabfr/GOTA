// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

#include "BuildingSettings.h"
#include "Population.h"
#include "ResourceStorage.h"
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
	DOREPLIFETIME_WITH_PARAMS(UBuilding, ProductionPerSecond, Params);
	DOREPLIFETIME_WITH_PARAMS(UBuilding, ResourceStorage, Params);
	DOREPLIFETIME_WITH_PARAMS(UBuilding, Efficiency, Params);
	DOREPLIFETIME_WITH_PARAMS(UBuilding, bIsProductionActive, Params);
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
	ResourceStorage = CreateDefaultSubobject<UResourceStorage>(TEXT("ResourceStorage"));
	ResourceStorage->OnIsEmptyChanged.AddDynamic(this, &UBuilding::S_HandleStorageEmptyChanged);
}

void UBuilding::S_Init(UBuildingSettings* InSettings, ATile* InTile, ASettlement* InSettlement)
{
	Population->S_Init(InSettlement->GetGrowthPerOwnPop(), InSettlement->GetGrowthPerNeighborPop());
	ResourceStorage->S_Init(InSettings->BaseResourceLimit, true, false);
	bIsProductionActive = !ResourceStorage->IsEmpty();
	Settings = InSettings;
	Tile = InTile;
	S_SetSettlement(InSettlement);
	bIsUnderConstruction = true;
	S_RecalculateProductionPerSecond();
}

void UBuilding::C_Init()
{
}

void UBuilding::S_Tick(const float DeltaSeconds)
{
	Population->S_Tick(DeltaSeconds);
	if (GetSettings()->bConsumptionEnabled && !bIsProductionActive)
	{
		ResourceStorage->S_Remove(GetSettings()->BaseConsumptionPerSecond * DeltaSeconds);
	}
}

void UBuilding::C_Tick(const float DeltaSeconds)
{
	Population->C_Tick(DeltaSeconds);
	if (GetSettings()->bConsumptionEnabled && !bIsProductionActive)
	{
		ResourceStorage->Remove(GetSettings()->BaseConsumptionPerSecond * DeltaSeconds);
	}
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
	S_RefreshEfficiency();
}

// --------------------------------------- Efficiency ---------------------------------------

void UBuilding::S_SetEfficiency(const float NewEfficiency)
{
	const float Change = NewEfficiency - Efficiency;
	if (FMath::IsNearlyZero(Change)) return;

	Efficiency = NewEfficiency;
	OnEfficiencyChanged.Broadcast(Change);
	S_RecalculateProductionPerSecond();
	ResourceStorage->S_SetLimit(GetSettings()->BaseResourceLimit +
		Efficiency * GetSettings()->ResourceLimitIncreasePerEfficiencyPercentage);
}

void UBuilding::S_RefreshEfficiency()
{
	const float PopulationFactor = Population->GetSize() / static_cast<float>(Settings->Housing);
	S_SetEfficiency(1.0f * PopulationFactor);
}

// --------------------- Construction phase ---------------------

FConstructionResources UBuilding::GetConstructionProgress() const
{
	return ConstructionProgress;
}

void UBuilding::S_SetConstructionProgress(const FConstructionResources NewConstructionProgress)
{
	ConstructionProgress = NewConstructionProgress;
	MARK_PROPERTY_DIRTY_FROM_NAME(UBuilding, ConstructionProgress, this)
	if (ConstructionProgress >= Settings->Cost)
		S_FinishConstruction();
}

void UBuilding::S_FinishConstruction()
{
	bIsUnderConstruction = false;
	MARK_PROPERTY_DIRTY_FROM_NAME(UBuilding, bIsUnderConstruction, this)
	Tile->OnBuildingFinishedConstruction();
	Population->S_ChangeMaxSize(Settings->Housing);
}

// --------------------- Production ---------------------

void UBuilding::S_RecalculateProductionPerSecond()
{
	float OldProductionPerSecond = ProductionPerSecond;
	if (GetSettings()->bProductionEnabled || !bIsProductionActive)
	{
		ProductionPerSecond = 0.f;
	}
	ProductionPerSecond = GetSettings()->BaseProductionPerSecond * GetEfficiency();
	if (ProductionPerSecond == OldProductionPerSecond) return;
	OnProductionPerSecondChanged.Broadcast(ProductionPerSecond - OldProductionPerSecond,
	                                       ProductionPerSecond, GetProductionType());
}

void UBuilding::OnRep_ProductionPerSecond(float OldProductionPerSecond)
{
	OnProductionPerSecondChanged.Broadcast(ProductionPerSecond - OldProductionPerSecond,
	                                       ProductionPerSecond, GetProductionType());
}

float UBuilding::GetProductionPerSecond() const
{
	return ProductionPerSecond;
}

EProductionType UBuilding::GetProductionType() const
{
	return GetSettings()->ProductionType;
}

void UBuilding::S_HandleStorageEmptyChanged(bool IsEmpty)
{
	bIsProductionActive = !IsEmpty;
	S_RecalculateProductionPerSecond();
}

// --------------------- Consumption ---------------------

// --------------------- Protection ---------------------

EResource UBuilding::GetConsumptionType() const
{
	return GetSettings()->ConsumptionType;
}

bool UBuilding::IsProtected() const
{
	TRACE_CPUPROFILER_EVENT_SCOPE_STR("UBuilding::IsProtected");
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

// --------------------- Army ---------------------

// --------------------- Defense building ---------------------
