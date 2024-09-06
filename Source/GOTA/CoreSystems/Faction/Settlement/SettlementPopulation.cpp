// Fill out your copyright notice in the Description page of Project Settings.

#include "SettlementPopulation.h"

// ------------------Tracking Changes----------------

void USettlementPopulation::RegisterPop(UPopulation* Pop)
{
	Pop->OnSizeChanged.AddDynamic(this, &USettlementPopulation::UpdateSize);
	UpdateSize(Pop->GetSize());
	Pop->OnMaxSizeChanged.AddDynamic(this, &USettlementPopulation::UpdateMaxSize);
	UpdateMaxSize(Pop->GetMaxSize());
	Pop->OnAngryChanged.AddDynamic(this, &USettlementPopulation::UpdateAngry);
	UpdateAngry(Pop->GetAngry());
	Pop->OnFearChanged.AddDynamic(this, &USettlementPopulation::UpdateFear);
	UpdateFear(Pop->GetFear());
	Populations.Add(Pop);
	/*
	if (!GameState) GameState = GetWorld()->GetGameState<AGS_Ingame>();
	if (Pop->GetFaction() == EFaction::Colonists)
		GameState->TotalColonialPopulation->RegisterPop(Pop);
	else if (Pop->GetFaction() == EFaction::Natives)
		GameState->TotalNativePopulation->RegisterPop(Pop);
		*/
}

void USettlementPopulation::UnregisterPop(UPopulation* Pop)
{
	Pop->OnSizeChanged.RemoveDynamic(this, &USettlementPopulation::UpdateSize);
	UpdateSize(-Pop->GetSize());
	Pop->OnMaxSizeChanged.RemoveDynamic(this, &USettlementPopulation::UpdateMaxSize);
	UpdateMaxSize(-Pop->GetMaxSize());
	Pop->OnAngryChanged.RemoveDynamic(this, &USettlementPopulation::UpdateAngry);
	UpdateAngry(-Pop->GetAngry());
	Pop->OnFearChanged.RemoveDynamic(this, &USettlementPopulation::UpdateFear);
	UpdateFear(-Pop->GetFear());
	Populations.Remove(Pop);
	/*
	if (Pop->GetFaction() == EFaction::Colonists)
		GameState->TotalColonialPopulation->UnregisterPop(Pop);
	else if (Pop->GetFaction() == EFaction::Natives)
		GameState->TotalNativePopulation->UnregisterPop(Pop);
	*/
}

void USettlementPopulation::UpdateSize(int16 ChangedBy)
{
	Size += ChangedBy;
}

void USettlementPopulation::UpdateMaxSize(int16 ChangedBy)
{
	MaxSize += ChangedBy;
}

void USettlementPopulation::UpdateAngry(int16 ChangedBy)
{
	Angry += ChangedBy;
}

void USettlementPopulation::UpdateFear(int16 ChangedBy)
{
	Fear += ChangedBy;
}
