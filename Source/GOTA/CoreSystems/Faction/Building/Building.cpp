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
	DOREPLIFETIME_WITH_PARAMS(UBuilding, IncomeProgress, Params);
	DOREPLIFETIME_WITH_PARAMS(UBuilding, Civilian, Params);
	DOREPLIFETIME_WITH_PARAMS(UBuilding, IsUnderConstruction, Params);
	DOREPLIFETIME_WITH_PARAMS(UBuilding, ResourceProgress, Params);
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
	if (Army) Army->GOTATick(DeltaSeconds);
	if(Settings->bIncomeEnabled)
	{
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
	// Army
	if (Settings->bArmyEnabled && !bIsUnderConstruction && !Army)
	{
		if (ArmyRespawnTimer < Settings->ArmyRespawnTime)
		{
			ArmyRespawnTimer += DeltaSeconds;
		}
		else if(Tile->AcceptsArmy())
		{
			ArmyRespawnTimer = 0.0f;
			Army = Tile->GetWorld()->SpawnActor<AArmy>();
			Army->Init(Settlement, Tile, GetArmyRecruitRate(), GetArmyMovementRate(), 1);
		}
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
	Settlement->S_AddResources(NewResources, true);
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

float UBuilding::GetCivilianWorkRate() const
{
	return 100 / Settings->SecondsPerWorkCycle;
}

float UBuilding::GetCivilianMovementRate() const
{
	return 100 / Settings->CivilianSecondsPerMove;
}

float UBuilding::GetArmyRecruitRate() const
{
	return 100 / Settings->SecondsPerRecruitCycle;
}

float UBuilding::GetArmyMovementRate() const
{
	return 100 / Settings->ArmySecondsPerMove;
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
	Population->ChangeMaxSize(Settings->Housing);
	if (Settings->bCivilianEnabled)
	{
		ACivilian* NewCivilian = Tile->GetWorld()->SpawnActor<ACivilian>(Settings->CivilianClass);
		NewCivilian->ServerInit(this, Settlement, Tile, GetCivilianWorkRate(), Settings->WorkAmountPerCycle,
					   GetCivilianMovementRate());
		SetCivilian(NewCivilian);
	}
}
