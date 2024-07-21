// Fill out your copyright notice in the Description page of Project Settings.


#include "PopulationSummary.h"

#include "AudioDeviceNotificationSubsystem.h"
#include "Net/UnrealNetwork.h"

void UPopulationSummary::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPopulationSummary, Current);
	DOREPLIFETIME(UPopulationSummary, Maximum);
	DOREPLIFETIME(UPopulationSummary, Follower);
	DOREPLIFETIME(UPopulationSummary, Moods);
}

bool UPopulationSummary::IsSupportedForNetworking() const
{
	return true;
}

UPopulationSummary::UPopulationSummary()
{
	Follower.Init(0, static_cast<int32>(EReligion::MAX));
	Moods.Init(0, static_cast<int32>(EMood::MAX));
}

void UPopulationSummary::OnRep_Current()
{
	OnPopulationChanged.Broadcast(0);
	OnChanged.Broadcast();
}

void UPopulationSummary::OnRep_Maximum()
{
	OnMaximumChanged.Broadcast(0);
	OnChanged.Broadcast();
}

void UPopulationSummary::OnRep_Follower()
{
	OnFollowerChanged.Broadcast(0,0,0,0,0);
	OnChanged.Broadcast();
}

void UPopulationSummary::OnRep_Moods()
{
	OnMoodChanged.Broadcast(0,0,0);
	OnChanged.Broadcast();
}

int32 UPopulationSummary::GetFollower(EReligion Religion) const
{
	return Follower[static_cast<int32>(Religion)];
}

int32 UPopulationSummary::GetFollowerNatives()
{
	int32 sum = 0;
	for (int i = 0; i <= 3; i++)
	{
		sum += Follower[i];
	}
	return sum;
}

void UPopulationSummary::GetAllFollower(int32& Guardian1, int32& Guardian2, int32& Guardian3, int32& Guardian4,
                                        int32& Colonists)
{
	Guardian1 = Follower[0];
	Guardian2 = Follower[1];
	Guardian3 = Follower[2];
	Guardian4 = Follower[3];
	Colonists = Follower[4];
}

int32 UPopulationSummary::GetMood(EMood Mood)
{
	return Moods[static_cast<int32>(Mood)];
}

void UPopulationSummary::GetAllMood(int32& Neutral, int32& Fearful, int32& Aggressive)
{
	Neutral = Moods[0];
	Fearful = Moods[1];
	Aggressive = Moods[2];
}

void UPopulationSummary::RegisterPopulation(UPopulation* Population)
{
	Population->OnPopulationChanged.AddDynamic(this, &UPopulationSummary::UpdatePopulation);
	Population->OnMaximumChanged.AddDynamic(this, &UPopulationSummary::UpdateMaximum);
	Population->OnFollowerChanged.AddDynamic(this, &UPopulationSummary::UpdateFollower);
	Population->OnMoodChanged.AddDynamic(this, &UPopulationSummary::UpdateMood);
}

void UPopulationSummary::RegisterPopulationSummary(UPopulationSummary* PopulationSummary)
{
	PopulationSummary->OnPopulationChanged.AddDynamic(this, &UPopulationSummary::UpdatePopulation);
	PopulationSummary->OnMaximumChanged.AddDynamic(this, &UPopulationSummary::UpdateMaximum);
	PopulationSummary->OnFollowerChanged.AddDynamic(this, &UPopulationSummary::UpdateFollower);
	PopulationSummary->OnMoodChanged.AddDynamic(this, &UPopulationSummary::UpdateMood);
}

void UPopulationSummary::UpdatePopulation(int32 Change)
{
	Current += Change;
	OnPopulationChanged.Broadcast(Change);
	OnChanged.Broadcast();
}

void UPopulationSummary::UpdateMaximum(int32 Change)
{
	Maximum += Change;
	OnMaximumChanged.Broadcast(Change);
	OnChanged.Broadcast();
}

void UPopulationSummary::UpdateFollower(int32 ChangeC, int32 ChangeG1, int32 ChangeG2, int32 ChangeG3, int32 ChangeG4)
{
	Follower[0] += ChangeC;
	Follower[1] += ChangeG1;
	Follower[2] += ChangeG2;
	Follower[3] += ChangeG3;
	Follower[4] += ChangeG4;
	OnFollowerChanged.Broadcast(
		ChangeC,
		ChangeG1,
		ChangeG2,
		ChangeG3,
		ChangeG4);
	OnChanged.Broadcast();
}

void UPopulationSummary::UpdateMood(int32 NeutralChange, int32 FearfulChange, int32 AggressiveChange)
{
	Moods[0] += NeutralChange;
	Moods[1] += FearfulChange;
	Moods[2] += AggressiveChange;
	OnMoodChanged.Broadcast(NeutralChange, FearfulChange, AggressiveChange);
	OnChanged.Broadcast();
}
