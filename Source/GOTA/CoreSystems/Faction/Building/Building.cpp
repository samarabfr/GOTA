// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

#include "BuildingDataAsset.h"
#include "Population.h"
#include "GOTA/CoreSystems/Entity/Civilian.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "Net/UnrealNetwork.h"

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

void UBuilding::ServerInit(UBuildingDataAsset* DataAsset_, ATile* Tile_, ASettlement* Settlement_)
{
	Settings = DataAsset_;
	Tile = Tile_;
	UnderConstructionTag = FGameplayTag::RequestGameplayTag(FName("Building.UnderConstruction"));
	Tile->GameplayTags.AddTag(UnderConstructionTag);
	IsUnderConstruction = true;
	Settlement = Settlement_;
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

FGameResources UBuilding::GetResourceProgress() const
{
	return ResourceProgress;
}

void UBuilding::SetResourceProgress(const FGameResources NewResourcesProgress)
{
	ResourceProgress = NewResourcesProgress;
	if(ResourceProgress >= Settings->Cost)
		FinishConstruction();
}

void UBuilding::FinishConstruction()
{
	IsUnderConstruction = false;
	Tile->GameplayTags.RemoveTag(UnderConstructionTag);
	Population->ChangeMaxSize(Settings->Housing);
	if (Settings->CivilianClass)
	{
		Civilian = Tile->GetWorld()->SpawnActor<ACivilian>(Settings->CivilianClass);
		Civilian->Init(Settlement, Tile, GetCivilianWorkRate(), Settings->WorkAmountPerCycle, GetCivilianMovementRate());
	}
}
