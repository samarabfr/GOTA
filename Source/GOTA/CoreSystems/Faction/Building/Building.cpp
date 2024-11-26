// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

#include "BuildingSettings.h"
#include "Population.h"
#include "GOTA/CoreSystems/Entity/Army.h"
#include "GOTA/CoreSystems/Entity/Civilian.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

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
	DOREPLIFETIME_WITH_PARAMS(UBuilding, DirectProductionProgress, Params);
	DOREPLIFETIME_WITH_PARAMS(UBuilding, Civilian, Params);
	DOREPLIFETIME_WITH_PARAMS(UBuilding, bIsUnderConstruction, Params);
	DOREPLIFETIME_WITH_PARAMS(UBuilding, ResourceProgress, Params);
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

void UBuilding::ServerTick(const float DeltaSeconds)
{
	Population->S_Tick(DeltaSeconds);
	if (Civilian)
		Civilian->S_Tick(DeltaSeconds);
	if (Army)
		Army->S_Tick(DeltaSeconds);
	if (Settings->bDirectProductionEnabled)
	{
		if (DirectProductionProgress < Settings->DirectProductionTime)
		{
			DirectProductionProgress += DeltaSeconds;
		}
		else
		{
			S_ApplyDirectProduction();
			DirectProductionProgress = 0.0f;
			MARK_PROPERTY_DIRTY_FROM_NAME(UBuilding, DirectProductionProgress, this)
		}
	}
	// Army
	if (Settings->bArmyEnabled && !bIsUnderConstruction && !Army)
	{
		if (ArmyRespawnTimer < Settings->ArmyRespawnTime)
		{
			ArmyRespawnTimer += DeltaSeconds;
		}
		else if (Tile->AcceptsArmy())
		{
			ArmyRespawnTimer = 0.0f;
			AArmy* NewArmy = Tile->GetWorld()->SpawnActor<AArmy>();
			NewArmy->S_Init(this, Tile);
			SetArmy(NewArmy);
		}
	}
}

void UBuilding::ClientTick(const float DeltaSeconds)
{
	Population->C_Tick(DeltaSeconds);
	if (Civilian)
		Civilian->C_Tick(DeltaSeconds);

	DirectProductionProgress += DeltaSeconds;
}

void UBuilding::ServerInit(UBuildingSettings* InSettings, ATile* InTile, ASettlement* InSettlement)
{
	Population->S_Init(InSettlement->GetPopulationSettings());
	Settings = InSettings;
	Tile = InTile;
	Settlement = InSettlement;
	Settlement->OnBuildingAdded(this, Tile);
	bIsUnderConstruction = true;
}

void UBuilding::ClientInit()
{
	Settlement->OnBuildingAdded(this, Tile);
}

void UBuilding::BeginDestroy()
{
	UObject::BeginDestroy();
	if (Settlement && Tile)
		Settlement->OnBuildingRemoved(this, Tile);
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

// --------------------------------------- Direct Production ---------------------------------------

void UBuilding::S_ApplyDirectProduction()
{
	FGameResources NewResources;
	switch (Settings->ProductionType)
	{
	case EProductionType::Food:
		NewResources.Food = Settings->DirectProductionAmount;
		break;

	case EProductionType::Wood:
		NewResources.Wood = Settings->DirectProductionAmount;
		break;

	case EProductionType::Stone:
		NewResources.Stone = Settings->DirectProductionAmount;
		break;
		
	default:
		break;
	}
	Settlement->S_AddResources(NewResources);
}

// ---------------- Army ----------------

void UBuilding::SetArmy(AArmy* NewArmy)
{
	Army = NewArmy;
	MARK_PROPERTY_DIRTY_FROM_NAME(UBuilding, Army, this)
}

// ---------------- Civilian ----------------

void UBuilding::SetCivilian(ACivilian* NewCivilian)
{
	Civilian = NewCivilian;
	MARK_PROPERTY_DIRTY_FROM_NAME(UBuilding, Civilian, this)
}

// --------------------- Construction phase ---------------------

FGameResources UBuilding::GetResourceProgress() const
{
	return ResourceProgress;
}

void UBuilding::SetResourceProgress(const FGameResources NewResourcesProgress)
{
	ResourceProgress = NewResourcesProgress;
	MARK_PROPERTY_DIRTY_FROM_NAME(UBuilding, ResourceProgress, this)
	if (ResourceProgress >= Settings->Cost)
		FinishConstruction();
}

void UBuilding::FinishConstruction()
{
	bIsUnderConstruction = false;
	MARK_PROPERTY_DIRTY_FROM_NAME(UBuilding, bIsUnderConstruction, this)
	Tile->OnBuildingFinishedConstruction();
	Population->S_ChangeMaxSize(Settings->Housing);
	if (Settings->bCivilianEnabled)
	{
		ACivilian* NewCivilian = Tile->GetWorld()->SpawnActor<ACivilian>(Settings->CivilianClass);
		NewCivilian->S_Init(this, Tile);
		SetCivilian(NewCivilian);
	}
}
