// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

#include "Production.h"
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
	DOREPLIFETIME_WITH_PARAMS(UBuilding, Production, Params);
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
	Production = CreateDefaultSubobject<UProduction>(TEXT("Production"));
	ResourceStorage = CreateDefaultSubobject<UResourceStorage>(TEXT("ResourceStorage"));
}

void UBuilding::S_Init(UBuildingSettings* InSettings, ATile* InTile, ASettlement* InSettlement)
{
	Population->S_Init(InSettlement->GetGrowthPerOwnPop(), InSettlement->GetGrowthPerNeighborPop());
	Production->S_Init(this);
	ResourceStorage->S_Init(GetSettings()->BaseResourceLimit, true, false);
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
	if (ResourceStorage->GetCurrent() <= 0.0f)
	{
		
	}
	else
	{
		ResourceStorage->S_Remove(GetSettings()->BaseConsumptionPerSecond * DeltaSeconds);
	}
}

void UBuilding::C_Tick(const float DeltaSeconds)
{
	Population->C_Tick(DeltaSeconds);
	ResourceStorage->S_Remove(GetSettings()->BaseConsumptionPerSecond * DeltaSeconds);
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
}

void UBuilding::RefreshEfficiency()
{
	SetEfficiency(Population->GetSize() / static_cast<float>(Settings->Housing));
}

// --------------------- Construction phase ---------------------

FConstructionResources UBuilding::GetConstructionProgress() const
{
	return ConstructionProgress;
}

void UBuilding::SetConstructionProgress(const FConstructionResources NewConstructionProgress)
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

UProduction* UBuilding::GetProduction()
{
	return Production;
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
