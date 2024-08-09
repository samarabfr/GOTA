// Fill out your copyright notice in the Description page of Project Settings.


#include "PopulationContainer.h"

#include "GOTA/CoreSystems/Faction/Settlement/SettlementBalance.h"
#include "Net/UnrealNetwork.h"


void UPopulationContainer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPopulationContainer, Population);
	DOREPLIFETIME(UPopulationContainer, Growth);
	DOREPLIFETIME(UPopulationContainer, GrowthChange);
	DOREPLIFETIME(UPopulationContainer, GrowthThreshold);
}

bool UPopulationContainer::IsSupportedForNetworking() const
{
	return true;
}

UPopulationContainer::UPopulationContainer()
{
	// Load the PopulationSettings asset, assuming it is stored in /Game/DataAssets/PopulationSettings
	static ConstructorHelpers::FObjectFinder<USettlementBalance> DataAsset(
		TEXT("/Game/CoreSystems/Faction/DA_SettlementBalance"));
	if (DataAsset.Succeeded())
	{
		USettlementBalance* SettlementBalance = DataAsset.Object;
		GrowthThreshold = SettlementBalance->PopulationGrowthThreshold;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Population couldn't load Settlement Balance Data Asset"))
	}
}

// ---------------------------------------------------------
// Population Struct

void UPopulationContainer::OnRep_Population(const FPopulation& OldPopulation)
{
	// TODO
}

// ---------------------------------------------------------
// Changing Population Values

void UPopulationContainer::ChangePopulationSize(int32 Change)
{
	if (Change > 0) // wants to increase pop
	{
		IncreasePopulationSize(Change);
	}
	else if (Change < 0) // wants to decrease pop
	{
		DecreasePopulationSize(FMath::Abs(Change));
	}
}

void UPopulationContainer::IncreasePopulationSize(int32 Change)
{
	if (Change <= 0) return; // invalid input
	FPopulation OldPop = Population;
	Population.Size += Change;
	if (Population.Size > Population.MaxSize) Population.Size = Population.MaxSize;
	int32 Effective_Change = Population.Size - OldPop.Size;
	if (Effective_Change == 0) return; // nothing happened

	for (int32 i = 0; i < Effective_Change; ++i)
	{
		AddOneFollowerWeightedRandom();
	}
	OnPopulationChanged.Broadcast(Population - OldPop);
}

void UPopulationContainer::DecreasePopulationSize(int32 Change)
{
	if (Change <= 0) return; // invalid input
	FPopulation OldPop = Population;
	Population.Size -= Change;
	if (Population.Size < 0) Population.Size = 0;
	int32 Effective_Change = Population.Size - OldPop.Size;
	if (Effective_Change == 0) return; // nothing happened

	for (int i = Effective_Change; i < 0; ++i)
	{
		SubtractOneFollowerWeightedRandom();
		SubtractOneMoodWeightedRandom();
	}
	OnPopulationChanged.Broadcast(Population - OldPop);
}

void UPopulationContainer::ChangeMaximum(int32 Change, int32& Effective_Change)
{
	int32 OldValue = Maximum;
	Maximum += Change;
	if (Maximum < 0) Maximum = 0;
	Effective_Change = Maximum - OldValue;

	if (Effective_Change == 0) return; // nothing happened

	if (Maximum < Current) // Maximum is smaller than pop so we have to reduce Pop
	{
		int32 E_C;
		ChangePopulation(Maximum - Change, E_C);
	}
	OnMaximumChanged.Broadcast(Effective_Change);
}

void UPopulationContainer::ChangeFollower(ECultureLoyalty Religion, int32 Change, int32& Effective_Change)
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

void UPopulationContainer::ChangeMood(EMood Mood, int32 Change, int32& Effective_Change)
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

	if (Mood == EMood::Angry && Effective_Change > 0)
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
	else if (Mood == EMood::Angry && Effective_Change < 0)
	{
		// If Aggressive gets reduced, increase Neutral
		Moods[0] += AbsoluteChange;
	}
	else if (Mood == EMood::Fear && Effective_Change > 0)
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
	else if (Mood == EMood::Fear && Effective_Change < 0)
	{
		// If Fearful gets reduced, increase Neutral
		Moods[0] += AbsoluteChange;
	}
	else if (Mood == EMood::Content && Effective_Change > 0)
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
	else if (Mood == EMood::Content && Effective_Change > 0)
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

void UPopulationContainer::AddOneFollowerWeightedRandom(ECultureLoyalty Exclude)
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

void UPopulationContainer::SubtractOneFollowerWeightedRandom(ECultureLoyalty Exclude)
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

void UPopulationContainer::AddOneFollowerToGuardiansFullRandom()
{
	Follower[FMath::RandRange(0, 3)]++;
}

void UPopulationContainer::SubtractOneMoodWeightedRandom(EMood Exclude)
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

// ---------------------------------------------------------
// Getters

FPopulation UPopulationContainer::GetPopulation()
{
	return Population;
}

int32 UPopulationContainer::GetFollower(ECultureLoyalty Religion) const
{
	switch (Religion)
	{
	case ECultureLoyalty::Colonists:
		return Population.FollowerColonists;
	case ECultureLoyalty::Guardian1:
		return Population.FollowerGuardian1;
	case ECultureLoyalty::Guardian2:
		return Population.FollowerGuardian2;
	case ECultureLoyalty::Guardian3:
		return Population.FollowerGuardian3;
	case ECultureLoyalty::Guardian4:
		return Population.FollowerGuardian4;
	default:
		return -1;
	}
}

int32 UPopulationContainer::GetFollowerNatives()
{
	return Population.GetNativeFollowers();
}

void UPopulationContainer::GetAllFollower(int32& Colonists, int32& Guardian1, int32& Guardian2, int32& Guardian3,
                                          int32& Guardian4)
{
	Colonists = Population.FollowerColonists;
	Guardian1 = Population.FollowerGuardian1;
	Guardian2 = Population.FollowerGuardian2;
	Guardian3 = Population.FollowerGuardian3;
	Guardian4 = Population.FollowerGuardian4;
}


int32 UPopulationContainer::GetMood(EMood Mood)
{
	switch (Mood)
	{
	case EMood::Content:
		return Population.GetContentMood();
	case EMood::Angry:
		return Population.MoodAngry;
	case EMood::Fear:
		return Population.MoodFear;
	default:
		return -1;
	}
}

void UPopulationContainer::GetAllMood(int32& Content, int32& Angry, int32& Fear)
{
	Content = Population.GetContentMood();
	Fear = Population.MoodFear;
	Angry = Population.MoodAngry;
}
