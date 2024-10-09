// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

#include "BuildingSettings.h"
#include "Population.h"
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
	DOREPLIFETIME_WITH_PARAMS(UBuilding, IncomeProgress, Params);
	DOREPLIFETIME_WITH_PARAMS(UBuilding, Civilian, Params);
}

bool UBuilding::IsSupportedForNetworking() const
{
	return true;
}

UBuilding::UBuilding()
{
	Population = CreateDefaultSubobject<UPopulation>(TEXT("Population"));
	Population->OnSizeChanged.AddDynamic(this, &UBuilding::ProductionChanged);
}

void UBuilding::ServerTick(const float DeltaSeconds)
{
	Population->ServerTick(DeltaSeconds);
	if (Civilian)
		Civilian->ServerTick(DeltaSeconds);
	if (IncomeProgress < Settings->IncomeTime)
	{
		IncomeProgress = FMath::Min(IncomeProgress + DeltaSeconds, Settings->IncomeTime);
	}
	else
	{
		AddIncomeToSettlement();
		IncomeProgress = 0.0f;
		MARK_PROPERTY_DIRTY_FROM_NAME(UBuilding, IncomeProgress, this)
	}
}

void UBuilding::ClientTick(const float DeltaSeconds)
{
	Population->ClientTick(DeltaSeconds);
	if (Civilian)
		Civilian->ClientTick(DeltaSeconds);

	IncomeProgress = FMath::Min(IncomeProgress + DeltaSeconds, Settings->IncomeTime);
}

void UBuilding::ServerInit(UBuildingSettings* InSettings, ATile* InTile, ASettlement* InSettlement)
{
	Settings = InSettings;
	Tile = InTile;
	Settlement = InSettlement;
	IsUnderConstruction = true;
	Settlement->OnBuildingAdded(this, Tile);
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

// --------------------- base income ---------------------

float UBuilding::GetCurrentIncomePerSecond() const
{
	return Settings->IncomeAmount * Population->GetSize() / Settings->IncomeTime;
}

void UBuilding::AddIncomeToSettlement()
{
	FGameResources NewResources;
	if (Settings->IncomeType == EProductionType::Food)
		NewResources.Food = Settings->IncomeAmount;
	if (Settings->IncomeType == EProductionType::Wood)
		NewResources.Wood = Settings->IncomeAmount;
	if (Settings->IncomeType == EProductionType::Stone)
		NewResources.Stone = Settings->IncomeAmount;
	Settlement->AddResources(NewResources, true);
}

void UBuilding::ProductionChanged(int16 Change)
{
	OnIncomeChanged.Broadcast(Settings->IncomeTime * Change, Settings->IncomeType);
}

// ---------------- Civilian Entity ----------------

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
	if (ResourceProgress >= Settings->Cost)
		FinishConstruction();
}

void UBuilding::FinishConstruction()
{
	IsUnderConstruction = false;
	Tile->OnBuildingFinishedConstruction();
	Population->ChangeMaxSize(Settings->Housing);
	if (Settings->CivilianClass)
	{
		ACivilian* NewCivilian = Tile->GetWorld()->SpawnActor<ACivilian>(Settings->CivilianClass);
		NewCivilian->ServerInit(this, Tile);
		SetCivilian(NewCivilian);
	}
}
