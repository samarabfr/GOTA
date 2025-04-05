// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

#include "BuildingSettings.h"
#include "Population.h"
#include "GOTA/Settlement/Settlement.h"
#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/Tile/Tile.h"
#include "GOTA/Tilemap/TileMap.h"
#include "GOTA/Utility/ResourceStorage.h"
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
	DOREPLIFETIME_WITH_PARAMS(UBuilding, Protectors, Params);
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

void UBuilding::S_Init(UBuildingSettings* InSettings, ATile* InTile, ASettlement* InSettlement, AGS_Ingame* InGameState)
{
	GameState = InGameState;
	Population->S_Init(InSettlement->GetGrowthPerOwnPop(), InSettlement->GetGrowthPerNeighborPop());
	ResourceStorage->S_Init(InSettings->BaseResourceLimit, true, false);
	bIsProductionActive = !ResourceStorage->IsEmpty();
	Settings = InSettings;
	Tile = InTile;
	S_SetSettlement(InSettlement);
	bIsUnderConstruction = true;
	S_RecalculateProductionPerSecond();
	S_CheckForProtection();
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

void UBuilding::PrepareDelete()
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
	if (!bIsUnderConstruction)
		return;
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

void UBuilding::S_CheckForProtection()
{
	if (GetSettings()->bDefenseEnabled || !GameState || !GameState->GetTileMap()) return;
	TArray<ATile*> Origin;
	Origin.Add(Tile);
	TMap<ATile*, int8> DefenseTilesWithDistances = GameState->GetTileMap()->FindAllTilesWithRangesInRange(Origin,
		MaxProtectionSearchRange, EEntityType::None,
		[](const ATile* Tile)
		{
			return Tile && Tile->GetBuilding() && Tile->GetBuilding()->Settings->bDefenseEnabled;
		});
	for (auto DefenseTile : DefenseTilesWithDistances)
	{
		if (DefenseTile.Key->GetClaimant() &&
			DefenseTile.Key->GetBuilding()->Settings->RavageProtectionRange >= DefenseTile.Value &&
			DefenseTile.Key->GetClaimant()->GetAffiliation() == Tile->GetClaimant()->GetAffiliation())
		{
			S_RegisterProtector(DefenseTile.Key->GetBuilding());
		}
	}
}

bool UBuilding::IsProtected() const
{
	if (GetSettings()->bDefenseEnabled)
		return false;
	return !Protectors.IsEmpty();
}

void UBuilding::S_RegisterProtector(UBuilding* Protector)
{
	if (GetSettings()->bDefenseEnabled)
		return;
	Protectors.Add(Protector);
}

void UBuilding::S_UnregisterProtector(UBuilding* Protector)
{
	if (GetSettings()->bDefenseEnabled)
		return;
	Protectors.Remove(Protector);
}

// --------------------- Army ---------------------

// --------------------- Defense building ---------------------
