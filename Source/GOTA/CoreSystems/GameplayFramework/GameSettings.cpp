// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

void UGameSettings::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	//	DOREPLIFETIME_WITH_PARAMS(UPopulation, Size, Params)
}

bool UGameSettings::IsSupportedForNetworking() const
{
	return true;
}

UGameSettings::UGameSettings()
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
