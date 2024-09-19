// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

#include "BuildingDataAsset.h"
#include "Population.h"
#include "GOTA/CoreSystems/Entity/Civilian.h"
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

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(UBuilding, Population, Params);
}

bool UBuilding::IsSupportedForNetworking() const
{
	return true;
}

UBuilding::UBuilding()
{
	Population = CreateDefaultSubobject<UPopulation>(TEXT("Population"));
	Population->OnSizeChanged.AddDynamic(this, &UBuilding::PopSizeChanged);
}

void UBuilding::ServerInit(UBuildingDataAsset* DataAsset_, ATile* Tile, ASettlement* Settlement)
{
	Settings = DataAsset_;
	Population->ChangeMaxSize(Settings->Housing);
	if (Settings->CivilianClass)
	{
		Civilian = Tile->GetWorld()->SpawnActor<ACivilian>(Settings->CivilianClass);
		Civilian->Init(Settlement, Tile, GetCivilianWorkRate(), Settings->WorkAmountPerCycle, GetCivilianMovementRate());
	}
}

float UBuilding::GetCurrentProduction() const
{
	return Settings->ProductionRate * Population->GetSize();
}

void UBuilding::PopSizeChanged(int16 Change)
{
	OnProductionChanged.Broadcast(Settings->ProductionRate * Change, Settings->ProductionType);
}

float UBuilding::GetCivilianWorkRate() const
{
	return 100 / Settings->SecondsPerWorkCycle;
}

float UBuilding::GetCivilianMovementRate() const
{
	return 100 / Settings->SecondsPerMove;
}
