// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

#include "BuildingSettings.h"
#include "Population.h"
#include "GOTA/CoreSystems/Entity/Civilian.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
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
	Population->OnSizeChanged.AddDynamic(this, &UBuilding::ProductionChanged);
}

void UBuilding::GOTATick(float DeltaSeconds)
{
	if(Civilian) Civilian->GOTATick(DeltaSeconds);
	if(IncomeProgress < Settings->IncomeTime)
	{
		IncomeProgress = FMath::Min(IncomeProgress + DeltaSeconds, Settings->IncomeTime);
	}
	else
	{
		AddIncomeToSettlement();
		IncomeProgress = 0.0f;
	}
}

void UBuilding::ServerInit(UBuildingSettings* DataAsset_, ATile* Tile_, ASettlement* Settlement_)
{
	Settings = DataAsset_;
	Tile = Tile_;
	UnderConstructionTag = FGameplayTag::RequestGameplayTag(FName("Building.UnderConstruction"));
	Tile->GameplayTags.AddTag(UnderConstructionTag);
	IsUnderConstruction = true;
	Settlement = Settlement_;
}

float UBuilding::GetCurrentIncomePerSecond() const
{
	return Settings->IncomeAmount * Population->GetSize() / Settings->IncomeTime;
}

void UBuilding::AddIncomeToSettlement()
{
	FGameResources NewResources;
	if(Settings->IncomeType == EProductionType::Food)
		NewResources.Food = Settings->IncomeAmount;
	if(Settings->IncomeType == EProductionType::Wood)
		NewResources.Wood = Settings->IncomeAmount;
	if(Settings->IncomeType == EProductionType::Stone)
		NewResources.Stone = Settings->IncomeAmount;
	Settlement->AddResources(NewResources, true);
}

void UBuilding::ProductionChanged(int16 Change)
{
	OnIncomeChanged.Broadcast(Settings->IncomeTime * Change, Settings->IncomeType);
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
