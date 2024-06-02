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
}

void UGOTAAttributePopulation::AddOneFollowerWeightedRandom(EReligion Exclude = EReligion::MAX)
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
	if(TotalBelievers == 0) return; // No Weights so this doesn't make sense
	
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

void UGOTAAttributePopulation::SubtractOneFollowerWeightedRandom(EReligion Exclude = EReligion::MAX)
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


void UGOTAAttributePopulation::OnChange()
{
	int32 Change = Current - OldValue;
	if (Change < 0) // Pop got reduced
	{
		for(int i = 0; i > Change; i--)
		{
			SubtractOneFollowerWeightedRandom();
		}
	}
	else if (Change > 0)
	{
		for(int i = 0; i < Change; i++)
		{
			AddOneFollowerWeightedRandom();
		}
	}
	OldValue = Current;
	OnChanged.Broadcast(Change);
}

void UGOTAAttributePopulation::ChangeFollower(EReligion Religion, int32 Change, int32& Effective_Change)
{
	if (Change == 0) return; // nothing happens...
	int32 SelectedIndex = static_cast<int32>(Religion);

	int32 OldFollower = Follower[SelectedIndex];
	Follower[SelectedIndex] = Follower[SelectedIndex] + Change;

	if (Follower[SelectedIndex] >= Current) // can't have more follower than pop
	{
		Follower[SelectedIndex] = Current;
		// all other Religions have 0 follower now
		for (int i = 0; i < static_cast<int32>(EReligion::MAX); i++)
		{
			if(i != SelectedIndex)
			{
				Follower[i] = 0;
			}
		}
		Effective_Change = Follower[SelectedIndex] - OldFollower;
		return;
	}
	if (Follower[SelectedIndex] < 0)
	{
		Follower[SelectedIndex] = 0;
	}
	
	Effective_Change = Follower[SelectedIndex] - OldFollower;
	if (Effective_Change > 0) // Increase
	{
		for(int i = 0; i < Effective_Change; i++)
		{
			SubtractOneFollowerWeightedRandom(Religion);
		}
	}
	else // Decrease
	{
		if(Religion == EReligion::Colonists) // Colonist get reduced
		{
			// Calculate how many Followers all other religion have
			int32 OtherReligionTotal = 0;
			for (int i = 0; i < static_cast<int32>(EReligion::MAX); i++)
			{
				if(SelectedIndex != i)
				{
					OtherReligionTotal += Follower[i];
				}
			}
			if(OtherReligionTotal == 0)
			{
				// Gibt noch keine Guardian follower, also muss full random genutzt werden
				for(int i = 0; i < Effective_Change; i++)
				{
					AddOneFollowerToGuardiansFullRandom();
				}
			} else
			{
				for(int i = 0; i < Effective_Change; i++)
				{
					AddOneFollowerWeightedRandom(Religion);
				}
			}
		} else // Guardian gets reduced, so colonist gets increased
		{
			Follower[static_cast<int32>(EReligion::Colonists)] += Effective_Change;
		}
	}
}

int32 UGOTAAttributePopulation::GetFollower(EReligion Religion) const
{
	return Follower[static_cast<int32>(Religion)];
}

int32 UGOTAAttributePopulation::GetFollowerNatives()
{
	int32 sum = 0;
	for(int i = 0; i <= 3; i++)
	{
		sum += Follower[i];
	}
	return sum;
}

void UGOTAAttributePopulation::GetAllFollowers(int32& Guardian1, int32& Guardian2, int32& Guardian3, int32& Guardian4,
	int32& Colonists)
{
	Guardian1 = Follower[0];
	Guardian2 = Follower[1];
	Guardian3 = Follower[2];
	Guardian4 = Follower[3];
	Colonists = Follower[4];
}