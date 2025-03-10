// Fill out your copyright notice in the Description page of Project Settings.

#include "SettlementPopulation.h"

#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/Tile/Building/Population.h"

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
}

void USettlementPopulation::StarveRandomPop()
{
	int32 RandomCursor = FMath::RandRange(0, GetSize());
	for (UPopulation* Pop : Populations)
	{
		RandomCursor -= Pop->GetSize();
		if(RandomCursor < 0)
		{
			Pop->S_ChangeSize(-1);
			return;
		}
	}
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

void USettlementPopulation::S_SetStarving(bool IsStarving)
{
	for (UPopulation* Pop : Populations)
	{
		Pop->S_SetIsStarving(IsStarving);
	}
}

void USettlementPopulation::S_SetGrowthPerOwnPop(float NewGrowthPerOwnPop)
{
	for (UPopulation* Pop : Populations)
	{
		Pop->S_SetGrowthPerOwnPop(NewGrowthPerOwnPop);
	}
}

void USettlementPopulation::S_SetGrowthPerNeighborPop(float NewGrowthPerNeighborPop)
{
	for (UPopulation* Pop : Populations)
	{
		Pop->S_SetGrowthPerNeighborPop(NewGrowthPerNeighborPop);
	}
}
