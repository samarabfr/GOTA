// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSettings.h"

#include "GOTA/CoreSystems/Faction/Building/PopulationSettings.h"
#include "GOTA/CoreSystems/Faction/Settlement/ColonyBrainSettings.h"
#include "GOTA/CoreSystems/Faction/Settlement/SettlementSettings.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// ------------------- Replication Setup -------------------

void AGameSettings::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(AGameSettings, TribePopulationSettings, Params)
	DOREPLIFETIME_WITH_PARAMS(AGameSettings, ColonyPopulationSettings, Params)

	DOREPLIFETIME_WITH_PARAMS(AGameSettings, TribeSettings, Params)
	DOREPLIFETIME_WITH_PARAMS(AGameSettings, ColonySettings, Params)
}

void AGameSettings::AddReplicatedSubObjects()
{
	AddReplicatedSubObject(TribePopulationSettings);
	AddReplicatedSubObject(ColonyPopulationSettings);
	AddReplicatedSubObject(TribeSettings);
	AddReplicatedSubObject(ColonySettings);
}

// ------------------- LifeCycle -------------------

AGameSettings::AGameSettings()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	NetUpdateFrequency = 0.5f;

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 1.0f;

	LoadPopulationSettings();
	LoadSettlementSettings();
}

void AGameSettings::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		AddReplicatedSubObjects();
	}
}

// ------------------- Population Settings -------------------

void AGameSettings::LoadPopulationSettings()
{
	const ConstructorHelpers::FObjectFinder<UPopulationSettingsDefaults> TribePopulationFinder(
		TEXT("/Game/CoreSystems/Faction/DA_TribePopulationSettings"));
	if (TribePopulationFinder.Succeeded())
	{
		TribePopulationSettings = DuplicateObject<UPopulationSettings>(
			TribePopulationFinder.Object->PopulationSettings, GetOuter());
	}

	const ConstructorHelpers::FObjectFinder<UPopulationSettingsDefaults> ColonyPopulationFinder(
		TEXT("/Game/CoreSystems/Faction/DA_ColonyPopulationSettings"));
	if (ColonyPopulationFinder.Succeeded())
	{
		ColonyPopulationSettings = DuplicateObject<UPopulationSettings>(
			ColonyPopulationFinder.Object->PopulationSettings, GetOuter());
	}
}

// ------------------- Settlement Settings -------------------

void AGameSettings::LoadSettlementSettings()
{
	const ConstructorHelpers::FObjectFinder<USettlementSettingsDefaults> TribeFinder(
		TEXT("/Game/CoreSystems/Faction/DA_TribeSettings"));
	if (TribeFinder.Succeeded())
	{
		TribeSettings = DuplicateObject<USettlementSettings>(
			TribeFinder.Object->SettlementSettings, GetOuter());
	}

	const ConstructorHelpers::FObjectFinder<USettlementSettingsDefaults> ColonyFinder(
		TEXT("/Game/CoreSystems/Faction/DA_ColonySettings"));
	if (ColonyFinder.Succeeded())
	{
		ColonySettings = DuplicateObject<USettlementSettings>(
			ColonyFinder.Object->SettlementSettings, GetOuter());
	}

	const ConstructorHelpers::FObjectFinder<UColonyBrainSettingsDefaults> ColonyBrainFinder(
	TEXT("/Game/CoreSystems/Faction/DA_ColonyBrainSettings"));
	if (ColonyBrainFinder.Succeeded())
	{
		ColonyBrainSettings = DuplicateObject<UColonyBrainSettings>(
			ColonyBrainFinder.Object->ColonyBrainSettings, GetOuter());
	}
}
