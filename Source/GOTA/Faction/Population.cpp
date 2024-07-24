// Fill out your copyright notice in the Description page of Project Settings.


#include "Population.h"
#include "Net/UnrealNetwork.h"


void UPopulation::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UPopulation, Current);
	DOREPLIFETIME(UPopulation, Maximum);
	DOREPLIFETIME(UPopulation, Follower);
	DOREPLIFETIME(UPopulation, Moods);
}

bool UPopulation::IsSupportedForNetworking() const
{
	return true;
}

UPopulation::UPopulation()
{
	Follower.Init(0, static_cast<int32>(ECultureLoyalty::MAX));
	Moods.Init(0, static_cast<int32>(EMood::MAX));
}

void UPopulation::OnRep_Current(int32 Change)
{
	OnPopulationChanged.Broadcast(Change);
	OnChanged.Broadcast();
}

int32 UPopulation::GetCurrent() const
{
	return Current;
}

void UPopulation::OnRep_Maximum(int32 Change)
{
	OnMaximumChanged.Broadcast(Change);
	OnChanged.Broadcast();
}

void UPopulation::ChangePopulation(int32 Change, int32& Effective_Change)
{
	int32 OldValue = Current;
	Current += Change;
	if (Current < 0) Current = 0;
	if (Current > Maximum) Current = Maximum;
	Effective_Change = Current - OldValue;

	if (Effective_Change == 0) return; // nothing happened

	// save current state of variables to be able to trigger the change delegates
	TArray<int32> FollowerBefore = Follower;
	TArray<int32> MoodsBefore = Moods;
	int32 AbsoluteChange = FMath::Abs(Effective_Change);

	if (OldValue == 0) // Initial Pop increase
	{
		Follower[0] += Effective_Change;
		Moods[0] += Effective_Change;
	}
	else if (Effective_Change < 0) // Pop got reduced
	{
		for (int i = 0; i > AbsoluteChange; i--)
		{
			SubtractOneFollowerWeightedRandom();
			SubtractOneMoodWeightedRandom();
		}
	}
	else if (Effective_Change > 0) // Pop got increased
	{
		for (int i = 0; i < AbsoluteChange; i++)
		{
			AddOneFollowerWeightedRandom();
			Moods[0]++;
		}
	}
	OnChanged.Broadcast();
	OnPopulationChanged.Broadcast(Effective_Change);
	OnFollowerChanged.Broadcast(
		Follower[0] - FollowerBefore[0],
		Follower[1] - FollowerBefore[1],
		Follower[2] - FollowerBefore[2],
		Follower[3] - FollowerBefore[3],
		Follower[4] - FollowerBefore[4]);
	OnMoodChanged.Broadcast(
		Moods[0] - MoodsBefore[0],
		Moods[1] - MoodsBefore[1],
		Moods[2] - MoodsBefore[2]);
}

void UPopulation::ChangeMaximum(int32 Change, int32& Effective_Change)
{
	int32 OldValue = Maximum;
	Maximum += Change;
	if (Maximum < 0) Maximum = 0;
	Effective_Change = Maximum - OldValue;

	if (Effective_Change == 0) return; // nothing happened

	if (Maximum > Current) // Maximum is smaller than pop so we have to reduce Pop
	{
		int32 E_C;
		ChangePopulation(Maximum - Change, E_C);
	}
	OnMaximumChanged.Broadcast(Effective_Change);
}

//====================================================================
//-------------------- Followers
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void UPopulation::AddOneFollowerWeightedRandom(ECultureLoyalty Exclude)
{
	int32 ExcludeIndex = static_cast<int32>(Exclude);
	int32 TotalBelievers = 0;
	// Calculate TotalBelievers in the selection pool
	for (int i = 0; i < static_cast<int32>(ECultureLoyalty::MAX); i++)
	{
		if (i != ExcludeIndex)
		{
			TotalBelievers += Follower[i];
		}
	}
	if (TotalBelievers == 0) return; // No Weights so this doesn't make sense

	// Select a random Believer
	int32 cursor = FMath::RandRange(0, TotalBelievers);
	// Find Selected Religion
	for (int i = 0; i < static_cast<int32>(ECultureLoyalty::MAX); i++)
	{
		if (i != ExcludeIndex && Follower[i] > 0)
		{
			cursor -= Follower[i];
			if (cursor <= 0)
			{
				// This Religion is selected
				Follower[i]++;
				return;
			}
		}
	}
}

void UPopulation::SubtractOneFollowerWeightedRandom(ECultureLoyalty Exclude)
{
	int32 ExcludeIndex = static_cast<int32>(Exclude);
	int32 TotalBelievers = 0;
	// Calculate TotalBelievers in the selection pool
	for (int i = 0; i < static_cast<int32>(ECultureLoyalty::MAX); i++)
	{
		if (i != ExcludeIndex)
		{
			TotalBelievers += Follower[i];
		}
	}
	// Select a random Believer
	int32 cursor = FMath::RandRange(0, TotalBelievers);
	// Find Selected Religion
	for (int i = 0; i < static_cast<int32>(ECultureLoyalty::MAX); i++)
	{
		// Exclude and don't use 0 Weight Religions
		if (i != ExcludeIndex && Follower[i] > 0)
		{
			cursor -= Follower[i];
			if (cursor <= 0)
			{
				// This Religion is selected
				Follower[i]--;
				return;
			}
		}
	}
}

void UPopulation::AddOneFollowerToGuardiansFullRandom()
{
	Follower[FMath::RandRange(0, 3)]++;
}

void UPopulation::OnRep_Follower()
{
	OnFollowerChanged.Broadcast(0,0,0,0,0);
	OnChanged.Broadcast();
}

void UPopulation::ChangeFollower(ECultureLoyalty Religion, int32 Change, int32& Effective_Change)
{
	if (Change == 0) return; // nothing happens...
	int32 SelectedIndex = static_cast<int32>(Religion);

	TArray<int32> OldFollower = Follower;
	Follower[SelectedIndex] = Follower[SelectedIndex] + Change;

	// Everyone follow Target religion now
	if (Follower[SelectedIndex] >= Current) Follower[SelectedIndex] = Current;

	// Target Religion has no followers now
	if (Follower[SelectedIndex] < 0) Follower[SelectedIndex] = 0;

	// Calculate how much Target Religion really changed
	Effective_Change = Follower[SelectedIndex] - OldFollower[SelectedIndex];
	if (Effective_Change == 0) return; // nothing happened...

	// Now we have to figure out what has to happen to the other Religions

	// If everyone follows Target Religion
	if (Follower[SelectedIndex] == Current)
	{
		// all other Religions have 0 follower now
		for (int i = 0; i < static_cast<int32>(ECultureLoyalty::MAX); i++)
		{
			if (i != SelectedIndex)
			{
				Follower[i] = 0;
			}
		}
	}
	// Target Religion Followers Increased
	else if (Effective_Change > 0)
	{
		for (int i = 0; i < Effective_Change; i++)
		{
			SubtractOneFollowerWeightedRandom(Religion);
		}
	}
	// Target Religion Followers Decreased
	else
	{
		// We need to distinguish if Target Religion is Colonist or not
		if (Religion == ECultureLoyalty::Colonists)
		{
			// Calculate how many Followers native religions have
			int32 OtherReligionTotal = 0;
			for (int i = 0; i < static_cast<int32>(ECultureLoyalty::MAX); i++)
			{
				if (SelectedIndex != i)
				{
					OtherReligionTotal += Follower[i];
				}
			}
			// If Nobody follows Guardians, Weighted Random doesn't work, so we use Full Random
			int32 AbsoluteChange = FMath::Abs(Effective_Change);
			if (OtherReligionTotal == 0)
			{
				for (int i = 0; i < AbsoluteChange; i++)
				{
					AddOneFollowerToGuardiansFullRandom();
				}
			}
			else // Otherwise Weighted Random works
			{
				for (int i = 0; i < AbsoluteChange; i++)
				{
					AddOneFollowerWeightedRandom(Religion);
				}
			}
		}
		else // Guardian gets reduced, so colonist gets increased
		{
			Follower[static_cast<int32>(ECultureLoyalty::Colonists)] += Effective_Change;
		}
	}
	OnFollowerChanged.Broadcast(
		Follower[0] - OldFollower[0],
		Follower[1] - OldFollower[1],
		Follower[2] - OldFollower[2],
		Follower[3] - OldFollower[3],
		Follower[4] - OldFollower[4]);
}

int32 UPopulation::GetFollower(ECultureLoyalty Religion) const
{
	return Follower[static_cast<int32>(Religion)];
}

int32 UPopulation::GetFollowerNatives()
{
	int32 sum = 0;
	for (int i = 0; i <= 3; i++)
	{
		sum += Follower[i];
	}
	return sum;
}

void UPopulation::GetAllFollower(int32& Guardian1, int32& Guardian2, int32& Guardian3, int32& Guardian4,
                                 int32& Colonists)
{
	Guardian1 = Follower[0];
	Guardian2 = Follower[1];
	Guardian3 = Follower[2];
	Guardian4 = Follower[3];
	Colonists = Follower[4];
}

void UPopulation::SubtractOneMoodWeightedRandom(EMood Exclude)
{
	int32 ExcludeIndex = static_cast<int32>(Exclude);
	int32 TotalMood = 0;
	// Calculate TotalMood in the selection pool
	for (int i = 0; i < static_cast<int32>(EMood::MAX); i++)
	{
		if (i != ExcludeIndex)
		{
			TotalMood += Moods[i];
		}
	}
	// Select a random dude with mood
	int32 cursor = FMath::RandRange(0, TotalMood);
	// Find Selected mood
	for (int i = 0; i < static_cast<int32>(EMood::MAX); i++)
	{
		// Exclude and don't use 0 Weight mood
		if (i != ExcludeIndex && Moods[i] > 0)
		{
			cursor -= Moods[i];
			if (cursor <= 0)
			{
				// This mood is selected
				Moods[i]--;
				return;
			}
		}
	}
}

void UPopulation::OnRep_Moods()
{
	OnMoodChanged.Broadcast(0,0,0);
	OnChanged.Broadcast();
}

void UPopulation::ChangeMood(EMood Mood, int32 Change, int32& Effective_Change)
{
	int32 MoodIndex = static_cast<int32>(Mood);
	TArray<int32> OldMoods = Moods;
	Moods[MoodIndex] += Change;

	// Mood can't be changed below 0
	if (Moods[MoodIndex] < 0) Moods[MoodIndex] = 0;
	// Mood can't be changed above current Population
	if (Moods[MoodIndex] > Current) Moods[MoodIndex] = Current;

	Effective_Change = Moods[MoodIndex] - OldMoods[MoodIndex];
	int32 AbsoluteChange = FMath::Abs(Effective_Change);

	if (Mood == EMood::Aggressive && Effective_Change > 0)
	{
		// If Aggressive gets Increased, first decrease Neutral, then Fearful pops
		if (Moods[0] >= AbsoluteChange)
		{
			Moods[0] -= AbsoluteChange;
		}
		else
		{
			AbsoluteChange -= Moods[0];
			Moods[0] = 0;
			Moods[1] -= AbsoluteChange;
		}
	}
	else if (Mood == EMood::Aggressive && Effective_Change < 0)
	{
		// If Aggressive gets reduced, increase Neutral
		Moods[0] += AbsoluteChange;
	}
	else if (Mood == EMood::Fearful && Effective_Change > 0)
	{
		// If Fearful gets Increased, first use Neutral, then Aggressive pops
		if (Moods[0] >= AbsoluteChange)
		{
			Moods[0] -= AbsoluteChange;
		}
		else
		{
			AbsoluteChange -= Moods[0];
			Moods[0] = 0;
			Moods[2] -= AbsoluteChange;
		}
	}
	else if (Mood == EMood::Fearful && Effective_Change < 0)
	{
		// If Fearful gets reduced, increase Neutral
		Moods[0] += AbsoluteChange;
	}
	else if (Mood == EMood::Neutral && Effective_Change > 0)
	{
		// If Neutral gets increased, use Random to reduce Fearful and Aggressive pops
		int32 Random = FMath::RandRange(0, AbsoluteChange);
		if (Random > Moods[1])
		{
			Random = Moods[1];
		}
		AbsoluteChange -= Random;
		Moods[1] -= Random;
		Moods[2] -= AbsoluteChange;
	}
	else if (Mood == EMood::Neutral && Effective_Change > 0)
	{
		// If Neutral gets reduced, use Random to increase Fearful and Aggressive pops
		int32 Random = FMath::RandRange(0, AbsoluteChange);
		AbsoluteChange -= Random;
		Moods[1] += Random;
		Moods[2] += AbsoluteChange;
	}
	OnMoodChanged.Broadcast(
		Moods[0] - OldMoods[0],
		Moods[1] - OldMoods[1],
		Moods[2] - OldMoods[2]);
}

int32 UPopulation::GetMood(EMood Mood)
{
	return Moods[static_cast<int32>(Mood)];
}

void UPopulation::GetAllMood(int32& Neutral, int32& Fearful, int32& Aggressive)
{
	Neutral = Moods[0];
	Fearful = Moods[1];
	Aggressive = Moods[2];
}