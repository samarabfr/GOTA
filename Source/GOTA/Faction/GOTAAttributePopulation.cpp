// Fill out your copyright notice in the Description page of Project Settings.


#include "GOTAAttributePopulation.h"
#include "Net/UnrealNetwork.h"

void UGOTAAttributePopulation::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGOTAAttributePopulation, Follower);
}

UGOTAAttributePopulation::UGOTAAttributePopulation()
{
	Follower.Init(0, static_cast<int32>(EReligion::MAX));
	Moods.Init(0, static_cast<int32>(EMood::MAX));
}

void UGOTAAttributePopulation::OnChange()
{
	int32 Change = Current - OldValue;
	if (Change < 0) // Pop got reduced
	{
		for (int i = 0; i > Change; i--)
		{
			SubtractOneFollowerWeightedRandom();
			SubtractOneMoodWeightedRandom();
		}
	}
	else if (Change > 0) // Pop got increased
	{
		for (int i = 0; i < Change; i++)
		{
			AddOneFollowerWeightedRandom();
			Moods[0]++;
		}
	}
	OldValue = Current;
	OnChanged.Broadcast(Change);
}

//====================================================================
//-------------------- Followers
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void UGOTAAttributePopulation::AddOneFollowerWeightedRandom(EReligion Exclude)
{
	int32 ExcludeIndex = static_cast<int32>(Exclude);
	int32 TotalBelievers = 0;
	// Calculate TotalBelievers in the selection pool
	for (int i = 0; i < static_cast<int32>(EReligion::MAX); i++)
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
	for (int i = 0; i < static_cast<int32>(EReligion::MAX); i++)
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

void UGOTAAttributePopulation::SubtractOneFollowerWeightedRandom(EReligion Exclude)
{
	int32 ExcludeIndex = static_cast<int32>(Exclude);
	int32 TotalBelievers = 0;
	// Calculate TotalBelievers in the selection pool
	for (int i = 0; i < static_cast<int32>(EReligion::MAX); i++)
	{
		if (i != ExcludeIndex)
		{
			TotalBelievers += Follower[i];
		}
	}
	// Select a random Believer
	int32 cursor = FMath::RandRange(0, TotalBelievers);
	// Find Selected Religion
	for (int i = 0; i < static_cast<int32>(EReligion::MAX); i++)
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

void UGOTAAttributePopulation::AddOneFollowerToGuardiansFullRandom()
{
	Follower[FMath::RandRange(0, 3)]++;
}


void UGOTAAttributePopulation::ChangeFollower(EReligion Religion, int32 Change, int32& Effective_Change)
{
	if (Change == 0) return; // nothing happens...
	int32 SelectedIndex = static_cast<int32>(Religion);

	int32 OldFollower = Follower[SelectedIndex];
	Follower[SelectedIndex] = Follower[SelectedIndex] + Change;

	// Everyone follow Target religion now
	if (Follower[SelectedIndex] >= Current)
	{
		Follower[SelectedIndex] = Current;
		// all other Religions have 0 follower now
		for (int i = 0; i < static_cast<int32>(EReligion::MAX); i++)
		{
			if (i != SelectedIndex)
			{
				Follower[i] = 0;
			}
		}
		Effective_Change = Follower[SelectedIndex] - OldFollower;
		return;
	}

	// Target Religion has no followers now
	if (Follower[SelectedIndex] < 0)
	{
		Follower[SelectedIndex] = 0;
	}

	// Calculate how much Target Religion really changed
	Effective_Change = Follower[SelectedIndex] - OldFollower;

	// Target Religion Followers Increased
	if (Effective_Change > 0)
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
		if (Religion == EReligion::Colonists)
		{
			// Calculate how many Followers all other religion have
			int32 OtherReligionTotal = 0;
			for (int i = 0; i < static_cast<int32>(EReligion::MAX); i++)
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
			Follower[static_cast<int32>(EReligion::Colonists)] += Effective_Change;
		}
	}
	if (Effective_Change != 0)
	{
		OnChange();
	}
}

int32 UGOTAAttributePopulation::GetFollower(EReligion Religion) const
{
	return Follower[static_cast<int32>(Religion)];
}

int32 UGOTAAttributePopulation::GetFollowerNatives()
{
	int32 sum = 0;
	for (int i = 0; i <= 3; i++)
	{
		sum += Follower[i];
	}
	return sum;
}

void UGOTAAttributePopulation::GetAllFollower(int32& Guardian1, int32& Guardian2, int32& Guardian3, int32& Guardian4,
                                              int32& Colonists)
{
	Guardian1 = Follower[0];
	Guardian2 = Follower[1];
	Guardian3 = Follower[2];
	Guardian4 = Follower[3];
	Colonists = Follower[4];
}

void UGOTAAttributePopulation::SubtractOneMoodWeightedRandom(EMood Exclude)
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

void UGOTAAttributePopulation::ChangeMood(EMood Mood, int32 Change, int32& Effective_Change)
{
	int32 MoodIndex = static_cast<int32>(Mood);
	int32 OldMood = Moods[MoodIndex];
	Moods[MoodIndex] += Change;
	// Mood can't be changed below 0
	if (Moods[MoodIndex] < 0)
	{
		Moods[MoodIndex] = 0;
	}
	// Mood can't be changed above current Population
	if (Moods[MoodIndex] > Current)
	{
		Moods[MoodIndex] = Current;
	}
	Effective_Change = Moods[MoodIndex] - OldMood;
	int32 AbsoluteChange = FMath::Abs(Effective_Change);


	if (Mood == EMood::Aggressive && Effective_Change > 0)
	{
		// If Aggressive gets Increased, first use Neutral, then Fearful pops
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
	OnChange();
}



int32 UGOTAAttributePopulation::GetMood(EMood Mood)
{
	return Moods[static_cast<int32>(Mood)];
}

void UGOTAAttributePopulation::GetAllMood(int32& Neutral, int32& Fearful, int32& Aggressive)
{
	Neutral = Moods[0];
	Fearful = Moods[1];
	Aggressive = Moods[2];
}