// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSettings.h"
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
}

void AGameSettings::AddReplicatedSubObjects()
{
	AddReplicatedSubObject(TribePopulationSettings);
	AddReplicatedSubObject(ColonyPopulationSettings);
}

// ------------------- LifeCycle -------------------

AGameSettings::AGameSettings()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	NetUpdateFrequency = 1.0f;

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 1.0f;
	
	LoadPopulationSettings();
}

void AGameSettings::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
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
		TribePopulationSettings = TribePopulationFinder.Object->PopulationSettings;

	const ConstructorHelpers::FObjectFinder<UPopulationSettingsDefaults> ColonyPopulationFinder(
		TEXT("/Game/CoreSystems/Faction/DA_ColonyPopulationSettings"));
	if (ColonyPopulationFinder.Succeeded())
		ColonyPopulationSettings = ColonyPopulationFinder.Object->PopulationSettings;
}
