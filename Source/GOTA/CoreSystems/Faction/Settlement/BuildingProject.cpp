// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingProject.h"
#include "Settlement.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "Net/UnrealNetwork.h"

void UBuildingProject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBuildingProject, Builder);
	DOREPLIFETIME(UBuildingProject, Data);
	DOREPLIFETIME(UBuildingProject, Tile);
	DOREPLIFETIME(UBuildingProject, Cost);
}

bool UBuildingProject::IsSupportedForNetworking() const
{
	return true;
}

bool UBuildingProject::IsPossible()
{
	if (!Builder) return false;
	if (!Tile) return false;

	//Tile is not claimed by the Builder of this project
	if (Tile->GetClaimant() != Builder) return false;

	//want to build a new building, but tile already has a building
	if (Tile->Building) return false;

	// Building takes infinitely long to build
	if (CalculateProjectTime() == MAX_int32) return false;

	return true;
}

bool UBuildingProject::CanAfford() const
{
	if (Cost.Wood > Builder->Resources.Wood) return false;
	if (Cost.Stone > Builder->Resources.Stone) return false;
	return true;
}

bool UBuildingProject::TryBuilding()
{
	// Trying to build a new building
	if (Tile->TryBuild(Data))
	{
		if (Cost.Wood > 0) Builder->Resources.Wood -= Cost.Wood;
		if (Cost.Stone > 0) Builder->Resources.Stone -= Cost.Stone;
		return true;
	}
	return false;
}

float UBuildingProject::CalculateScore()
{
	// costs
	float ProjectTime = CalculateProjectTime();
	const float CostScore = FMath::Pow(EULERS_NUMBER, -0.1 * ProjectTime);
	// gains
	float GainsScore = 0;
	// Calculate GainScore
	// income
	const float MaxIncome = Data->Housing *  Data->ProductionRate;
	if (Data->ProductionType == EProductionType::Food)
	{
		GainsScore = Builder->ImportanceRatings.Food * MaxIncome;
	}
	if (Data->ProductionType == EProductionType::Wood)
	{
		GainsScore = Builder->ImportanceRatings.Wood * MaxIncome;
	}
	if (Data->ProductionType == EProductionType::Stone)
	{
		GainsScore = Builder->ImportanceRatings.Food * MaxIncome;
	}

	// result
	return CostScore * GainsScore;
}

int32 UBuildingProject::CalculateProjectTime()
{
	// TODO: make functions of income calcs
	TArray<int32> Times;
	// Time to get all the food
	int32 FoodTime = MAX_int32;
	float FoodIncome = Builder->BuildingSummary->ProductionMap[EProductionType::Food];
	const float MoreFoodNeeded = FMath::Max(0, Cost.Food - Builder->Resources.Food);
	if (MoreFoodNeeded == 0)
		FoodTime = 0;
	else if (MoreFoodNeeded > 0 && FoodIncome > 0)
		FoodTime = FMath::RoundFromZero(MoreFoodNeeded / FoodIncome);
	Times.Add(FoodTime);
	// Time to get all the wood
	int32 WoodTime = MAX_int32;
	float WoodIncome = Builder->BuildingSummary->ProductionMap[EProductionType::Wood];
	const float MoreWoodNeeded = FMath::Max(0, Cost.Wood - Builder->Resources.Wood);
	if (MoreWoodNeeded == 0)
		WoodTime = 0;
	else if (MoreWoodNeeded > 0 && WoodIncome > 0)
		WoodTime = FMath::RoundFromZero(MoreWoodNeeded / WoodIncome);
	Times.Add(WoodTime);
	// Time to get all the stone
	int32 StoneTime = MAX_int32;
	float StoneIncome = Builder->BuildingSummary->ProductionMap[EProductionType::Stone];
	const float MoreStoneNeeded = FMath::Max(0, Cost.Stone - Builder->Resources.Stone);
	if (MoreStoneNeeded == 0)
		StoneTime = 0;
	else if (MoreStoneNeeded > 0 && StoneIncome > 0)
		StoneTime = FMath::RoundFromZero(MoreStoneNeeded / StoneIncome);
	Times.Add(StoneTime);
	// Max individual time is project time
	return FMath::Max(Times);
}

void UBuildingProject::Init(ASettlement* Builder_, UBuildingDataAsset* Data_, ATile* Tile_)
{
	Builder = Builder_;
	Data = Data_;
	Tile = Tile_;
	Cost = Data->Cost;
}
