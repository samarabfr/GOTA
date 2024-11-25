// Fill out your copyright notice in the Description page of Project Settings.

#include "SettlementSettings.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// ------------------- Replication Setup -------------------

void USettlementSettings::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(USettlementSettings, PopEatingPerSecond, Params)
	DOREPLIFETIME_WITH_PARAMS(USettlementSettings, StarvingThreshold, Params)
}

bool USettlementSettings::IsSupportedForNetworking() const
{
	return true;
}

// ------------------- Init only Settings -------------------

// ------------------- Changeable Settings -------------------

void USettlementSettings::OnRep_PopEatingPerSecond(const float OldValue)
{
	OnPopEatingPerSecondChanged.Broadcast(PopEatingPerSecond - OldValue);
}

void USettlementSettings::S_SetPopEatingPerSecond(const float NewValue)
{
	const float Change = NewValue - PopEatingPerSecond;
	PopEatingPerSecond = NewValue;
	OnPopEatingPerSecondChanged.Broadcast(Change);
	MARK_PROPERTY_DIRTY_FROM_NAME(USettlementSettings, PopEatingPerSecond, this)
}

void USettlementSettings::S_SetStarvingThreshold(const float NewValue)
{
	StarvingThreshold = NewValue;
	MARK_PROPERTY_DIRTY_FROM_NAME(USettlementSettings, StarvingThreshold, this)
}

// ------------------- Defaults Data Asset -------------------

USettlementSettingsDefaults::USettlementSettingsDefaults()
{
	SettlementSettings = CreateDefaultSubobject<USettlementSettings>("Settlement Settings");
}
