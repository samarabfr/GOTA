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
	DOREPLIFETIME(UBuildingProject, Tier);
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
	if (Tier < 1) return false;

	//Tile is not claimed by the Builder of this project
	if (Tile->GetClaimant() != Builder) return false;

	//want to build a new building, but tile already has a building
	if (Tier == 1 && Tile->Building) return false;

	//want to upgrade a building but there is no building
	if (Tier > 1 && !Tile->Building) return false;

	//want to upgrade a building, but the tier of the building is not the previous tier of this project
	if (Tier > 1 && Tile->Building->Tier != Tier - 1) return false;

	// Building takes infinitely long to build
	if (CalculateProjectTime() == MAX_int32) return false;

	return true;
}

bool UBuildingProject::CanAfford() const
{
	if (Cost.Wood > Builder->Wood->Current) return false;
	if (Cost.Stone > Builder->Stone->Current) return false;
	return true;
}

bool UBuildingProject::TryBuilding()
{
	// Trying to build a new building
	if (Tier == 1)
	{
		if (Tile->TryBuild(Data))
		{
			int32 EC = 0;
			if (Cost.Wood > 0) Builder->Wood->Subtract(Cost.Wood, EC);
			if (Cost.Stone > 0) Builder->Stone->Subtract(Cost.Stone, EC);
			return true;
		}
	}
	// Trying to upgrade a Building
	if (Tile->TryUpgrade())
	{
		int32 EC = 0;
		if (Cost.Wood > 0) Builder->Wood->Subtract(Cost.Wood, EC);
		if (Cost.Stone > 0) Builder->Stone->Subtract(Cost.Stone, EC);
		return true;
	}
	return false;
}

int32 UBuildingProject::CalculateScore()
{
	// costs
	int32 ProjectTime = CalculateProjectTime();
	const float CostScore = ProjectTime;
	// gains
	float GainsScore = 0;
	FBuildingTierData* TierData = Data->GetTierData(Tier);
	if (TierData && TierData->TierEnabled)
	{
		const int32 CountThresholdMet = TierData->Housing / TierData->PopulationThreshold;
		int32 MaxIncome = CountThresholdMet * TierData->ProductionPerThreshold;
		// subtract income from previous tier
		if(Tier > 1)
		{
			FBuildingTierData* PreviousTierData = Data->GetTierData(Tier - 1);
			if (PreviousTierData && PreviousTierData->TierEnabled)
			{
				const int32 PreviousCountThresholdMet = PreviousTierData->Housing / PreviousTierData->PopulationThreshold;
				const int32 PreviousMaxIncome = PreviousCountThresholdMet * PreviousTierData->ProductionPerThreshold;
				MaxIncome -= PreviousMaxIncome;
			}
		}
		// Calculate GainScore
		if (TierData->ProductionType == EProductionType::Foraging)
		{
			const float MaxFoodIncome = MaxIncome * Builder->SettlementBalance->ForagingFoodToWoodRatio;
			const float MaxWoodIncome = MaxIncome * (1 - Builder->SettlementBalance->ForagingFoodToWoodRatio);
			GainsScore = Builder->ImportanceRatings.Food * MaxFoodIncome;
			GainsScore += Builder->ImportanceRatings.Wood * MaxWoodIncome;
		}
		else if (TierData->ProductionType == EProductionType::Woodcutting)
		{
			GainsScore = Builder->ImportanceRatings.Wood * MaxIncome;
		}
		else if (TierData->ProductionType == EProductionType::Hunting)
		{
			GainsScore = Builder->ImportanceRatings.Food * MaxIncome;
		}
		else if (TierData->ProductionType == EProductionType::Stonecutting)
		{
			GainsScore = Builder->ImportanceRatings.Stone * MaxIncome;
		}
		// TODO: incorporate housing
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
	float FoodIncome = Builder->BuildingSummary->ProductionMap[EProductionType::Hunting];
	FoodIncome += Builder->BuildingSummary->ProductionMap[EProductionType::Foraging]
		* Builder->SettlementBalance->ForagingFoodToWoodRatio;
	const float MoreFoodNeeded = FMath::Max(0, Cost.Food - Builder->Food->Current);
	if (MoreFoodNeeded == 0)
		FoodTime = 0;
	else if (MoreFoodNeeded > 0 && FoodIncome > 0)
		FoodTime = FMath::RoundFromZero(MoreFoodNeeded / FoodIncome);
	Times.Add(FoodTime);
	// Time to get all the wood
	int32 WoodTime = MAX_int32;
	float WoodIncome = Builder->BuildingSummary->ProductionMap[EProductionType::Woodcutting];
	WoodIncome += Builder->BuildingSummary->ProductionMap[EProductionType::Foraging]
		* (1 - Builder->SettlementBalance->ForagingFoodToWoodRatio);
	const float MoreWoodNeeded = FMath::Max(0, Cost.Wood - Builder->Wood->Current);
	if (MoreWoodNeeded == 0)
		WoodTime = 0;
	else if (MoreWoodNeeded > 0 && WoodIncome > 0)
		WoodTime = FMath::RoundFromZero(MoreWoodNeeded / WoodIncome);
	Times.Add(WoodTime);
	// Time to get all the stone
	int32 StoneTime = MAX_int32;
	float StoneIncome = Builder->BuildingSummary->ProductionMap[EProductionType::Stonecutting];
	const float MoreStoneNeeded = FMath::Max(0, Cost.Stone - Builder->Stone->Current);
	if (MoreStoneNeeded == 0)
		StoneTime = 0;
	else if (MoreStoneNeeded > 0 && StoneIncome > 0)
		StoneTime = FMath::RoundFromZero(MoreStoneNeeded / StoneIncome);
	Times.Add(StoneTime);
	// Max individual time is project time
	return FMath::Max(Times);
}

void UBuildingProject::Init(ASettlement* Builder_, UBuildingDataAsset* Data_, int32 Tier_, ATile* Tile_)
{
	Builder = Builder_;
	Data = Data_;
	Tier = Tier_;
	Tile = Tile_;
	Cost = Data->GetTierData(Tier)->Cost;
}
