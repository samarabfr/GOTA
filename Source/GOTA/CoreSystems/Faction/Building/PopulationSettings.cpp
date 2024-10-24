// Fill out your copyright notice in the Description page of Project Settings.

#include "PopulationSettings.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// ------------------- Replication Setup -------------------

void UPopulationSettings::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	
	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(UPopulationSettings, GrowthPerOwnPop, Params)
	DOREPLIFETIME_WITH_PARAMS(UPopulationSettings, GrowthPerNeighborPop, Params)
}

bool UPopulationSettings::IsSupportedForNetworking() const
{
	return true;
}

// ------------------- Settings -------------------

void UPopulationSettings::S_SetGrowthPerOwnPop(const float NewGrowthPerOwnPop)
{
	GrowthPerOwnPop = NewGrowthPerOwnPop;
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulationSettings, GrowthPerOwnPop, this)
}

void UPopulationSettings::S_SetGrowthPerNeighborPop(const float NewGrowthPerNeighborPop)
{
	GrowthPerNeighborPop = NewGrowthPerNeighborPop;
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulationSettings, GrowthPerNeighborPop, this)
}

// ------------------- Defaults Data Asset -------------------

UPopulationSettingsDefaults::UPopulationSettingsDefaults()
{
	PopulationSettings = CreateDefaultSubobject<UPopulationSettings>("Population Settings");
}
