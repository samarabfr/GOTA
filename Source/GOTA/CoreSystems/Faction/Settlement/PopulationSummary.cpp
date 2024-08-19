// Fill out your copyright notice in the Description page of Project Settings.


#include "PopulationSummary.h"
#include "Net/UnrealNetwork.h"

void UPopulationSummary::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPopulationSummary, Population);
}

bool UPopulationSummary::IsSupportedForNetworking() const
{
	return true;
}

void UPopulationSummary::OnRep_Population(const FPopulation& OldPopulation)
{
	OnPopulationChanged.Broadcast(Population - OldPopulation);
}

FPopulation UPopulationSummary::ExtractArmyPopulation(USettlementBalance* Balance)
{
	FPopulation Return;
	TArray<UPopulationContainer*> EligiblePopCons;
	TArray<int32> EligiblePop;
	int32 EligiblePopTotal = 0;
	for (UPopulationContainer* PopCon : PopCons)
	{
		if (PopCon->GetSize() >= Balance->MinBuildingPopToJoinArmy)
		{
			EligiblePopCons.Add(PopCon);
			int32 Eligible = PopCon->GetSize() - Balance->MinBuildingPopRemainingAfterJoining;
			EligiblePop.Add(Eligible);
			EligiblePopTotal += Eligible;
		}
	}
	if (EligiblePopTotal <= 0 || PopCons.IsEmpty()) return FPopulation();
	//figure out how big the army should be
	float ArmyPercentage = FMath::RandRange(Balance->MinRatioOfEligiblePopJoiningArmy,
	                                        Balance->MaxRatioOfEligiblePopJoiningArmy);
	int32 ArmySize = EligiblePopTotal * ArmyPercentage + Population.MoodAngry * Balance->BonusArmySizePerAngryPop;
	if (ArmySize > EligiblePopTotal)ArmySize = EligiblePopTotal;
	//figure out where we should remove pop
	int32 SelectedPops = 0;
	TArray<int32> SelectedPopsArray;
	SelectedPopsArray.SetNumZeroed(EligiblePop.Num());
	while (SelectedPops < ArmySize)
	{
		// Weighted Random to select a Pop for army joining
		int32 cursor = FMath::RandRange(0, EligiblePopTotal - 1);
		for (int32 i = 0; i < EligiblePop.Num(); i++)
		{
			cursor -= EligiblePop[i];
			if (cursor < 0)
			{
				++SelectedPopsArray[i];
				++SelectedPops;
				--EligiblePop[i];
				--EligiblePopTotal;
				break;
			}
		}
	}
	for (int i = 0; i < SelectedPopsArray.Num(); ++i)
	{
		Return += EligiblePopCons[i]->ExtractRandomPopForArmy(SelectedPopsArray[i], Balance);
	}
	return Return;
}

// ---------------------------------------------------------
// Keeping Track of Population Changes

void UPopulationSummary::RegisterPopulationContainer(UPopulationContainer* PopulationContainer)
{
	PopulationContainer->OnPopulationChanged.AddDynamic(this, &UPopulationSummary::UpdatePopulation);
	PopulationContainer->DefaultCulture = DefaultCulture;
	UpdatePopulation(PopulationContainer->Population);
	PopCons.Add(PopulationContainer);
}

void UPopulationSummary::RegisterPopulationSummary(UPopulationSummary* PopulationSummary)
{
	PopulationSummary->OnPopulationChanged.AddDynamic(this, &UPopulationSummary::UpdatePopulation);
	UpdatePopulation(PopulationSummary->Population);
}

void UPopulationSummary::UnregisterPopulationContainer(UPopulationContainer* PopulationContainer)
{
	PopulationContainer->OnPopulationChanged.RemoveDynamic(this, &UPopulationSummary::UpdatePopulation);
	UpdatePopulation(-PopulationContainer->Population);
	PopCons.Remove(PopulationContainer);
}

void UPopulationSummary::UnregisterPopulationSummary(UPopulationSummary* PopulationSummary)
{
	PopulationSummary->OnPopulationChanged.RemoveDynamic(this, &UPopulationSummary::UpdatePopulation);
	UpdatePopulation(-PopulationSummary->Population);
}

void UPopulationSummary::UpdatePopulation(FPopulation Change)
{
	Population += Change;
	ECultureLoyalty NewLargest = Population.GetLargestCulture();
	if (NewLargest != DefaultCulture)SetDefaultCulture(NewLargest);
	OnPopulationChanged.Broadcast(Change);
}

void UPopulationSummary::SetDefaultCulture(ECultureLoyalty Culture)
{
	DefaultCulture = Culture;
	for (UPopulationContainer* PopCon : PopCons)
	{
		PopCon->DefaultCulture = Culture;
	}
}

// ---------------------------------------------------------
// Getters

int32 UPopulationSummary::GetFollower(ECultureLoyalty Culture) const
{
	return Population.GetFollower(Culture);
}

int32 UPopulationSummary::GetNativeFollowers()
{
	return Population.GetNativeFollowers();
}

void UPopulationSummary::GetAllFollower(int32& Guardian1, int32& Guardian2, int32& Guardian3, int32& Guardian4,
                                        int32& Colonists)
{
	Colonists = Population.FollowerColonists;
	Guardian1 = Population.FollowerGuardian1;
	Guardian2 = Population.FollowerGuardian2;
	Guardian3 = Population.FollowerGuardian3;
	Guardian4 = Population.FollowerGuardian4;
}

int32 UPopulationSummary::GetMood(EMood Mood)
{
	return Population.GetMood(Mood);
}

void UPopulationSummary::GetAllMood(int32& Content, int32& Angry, int32& Fear)
{
	Content = Population.MoodContent;
	Angry = Population.MoodAngry;
	Fear = Population.MoodFear;
}
