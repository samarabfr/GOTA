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

// ---------------------------------------------------------
// Keeping Track of Population Changes

void UPopulationSummary::RegisterPopulationContainer(UPopulationContainer* PopulationContainer)
{
	PopulationContainer->OnPopulationChanged.AddDynamic(this, &UPopulationSummary::UpdatePopulation);
	OnPopulationChanged.Broadcast(PopulationContainer->Population);
	Population += PopulationContainer->Population;
}

void UPopulationSummary::RegisterPopulationSummary(UPopulationSummary* PopulationSummary)
{
	PopulationSummary->OnPopulationChanged.AddDynamic(this, &UPopulationSummary::UpdatePopulation);
	OnPopulationChanged.Broadcast(PopulationSummary->Population);
	Population += PopulationSummary->Population;
}

void UPopulationSummary::UnregisterPopulationContainer(UPopulationContainer* PopulationContainer)
{
	PopulationContainer->OnPopulationChanged.RemoveDynamic(this, &UPopulationSummary::UpdatePopulation);
	OnPopulationChanged.Broadcast(-PopulationContainer->Population);
	Population -= PopulationContainer->Population;
}

void UPopulationSummary::UnregisterPopulationSummary(UPopulationSummary* PopulationSummary)
{
	PopulationSummary->OnPopulationChanged.RemoveDynamic(this, &UPopulationSummary::UpdatePopulation);
	OnPopulationChanged.Broadcast(-PopulationSummary->Population);
	Population -= PopulationSummary->Population;
}

void UPopulationSummary::UpdatePopulation(FPopulation Change)
{
	Population += Change;
	OnPopulationChanged.Broadcast(Change);
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
