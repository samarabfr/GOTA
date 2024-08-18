// Fill out your copyright notice in the Description page of Project Settings.


#include "PopulationContainer.h"

#include "GOTA/CoreSystems/Faction/Settlement/SettlementBalance.h"
#include "GOTA/CoreSystems/GameplayFramework/GameBalance.h"
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
	// Load SettlementBalance to extract GrowthThreshold
	static ConstructorHelpers::FObjectFinder<USettlementBalance> DataAsset(
		TEXT("/Game/CoreSystems/Faction/DA_SettlementBalance"));
	if (DataAsset.Succeeded())
	{
		USettlementBalance* SettlementBalance = DataAsset.Object;
		GrowthThreshold = SettlementBalance->PopulationGrowthThreshold;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Population Container couldn't load Settlement Balance Data Asset"))
	}

	// Load GameBalance for Combat Value calculation
	static ConstructorHelpers::FObjectFinder<UGameBalanceDataAsset> DataAsset2(
		TEXT("/Game/CoreSystems/GameplayFramework/DA_GameBalance"));
	if (DataAsset2.Succeeded())
	{
		GameBalance = DataAsset2.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Population Container couldn't load GameBalance Data Asset"))
	}
}

// ---------------------------------------------------------
// Population Struct

void UPopulationContainer::OnRep_Population(const FPopulation& OldPopulation)
{
	OnPopulationChanged.Broadcast(Population - OldPopulation);
}

// ---------------------------------------------------------
// Combat Values

void UPopulationContainer::RecalculateCombatValues()
{
	HP = Population.Size * GameBalance->HumanHP;
	Attack = Population.MoodContent * GameBalance->ContentPopAttack;
	Attack += Population.MoodAngry * GameBalance->AngryPopAttack;
	Attack += Population.MoodFear * GameBalance->FearPopAttack;
	Attack += Population.Muskets * GameBalance->MusketAttack;
	Attack += Population.Bows * GameBalance->BowAttack;

	Defense = Population.MoodContent * GameBalance->ContentPopDefense;
	Defense += Population.MoodAngry * GameBalance->AngryPopDefense;
	Defense += Population.MoodFear * GameBalance->FearPopDefense;
	Defense += Population.MoodFear * GameBalance->ShieldDefense;
	OnCombatValuesChanged.Broadcast(HP, Attack, Defense);
}

// ---------------------------------------------------------
// Changing Population Values

void UPopulationContainer::AddPopulation(const FPopulation& Pop)
{
	Population += Pop;
	PopulationChanged(Pop);
}

void UPopulationContainer::ChangeSize(int32 Change)
{
	if (Change > 0)
	{
		IncreaseSize(Change);
	}
	else if (Change < 0)
	{
		DecreaseSize(-Change);
	}
}

void UPopulationContainer::IncreaseSize(int32 Change)
{
	if (Change <= 0) return; // invalid input
	FPopulation OldPop = Population;
	Population.Size += Change;
	if (Population.Size > Population.MaxSize) Population.Size = Population.MaxSize;
	int32 Effective_Change = Population.Size - OldPop.Size;
	if (Effective_Change == 0) return; // nothing happened

	for (int32 i = 0; i < Effective_Change; ++i)
	{
		ChangeFollowerWeightedRandomBy(1);
	}
	Population.MoodContent += Effective_Change;
	PopulationChanged(Population - OldPop);
}

void UPopulationContainer::DecreaseSize(int32 Change)
{
	if (Change <= 0) return; // invalid input
	FPopulation OldPop = Population;
	Population.Size -= Change;
	if (Population.Size < 0) Population.Size = 0;
	int32 Effective_Change = Population.Size - OldPop.Size;
	if (Effective_Change == 0) return; // nothing happened

	for (int32 i = Effective_Change; i < 0; ++i)
	{
		ChangeFollowerWeightedRandomBy(-1);
		SubtractOneMoodWeightedRandom();
	}
	PopulationChanged(Population - OldPop);
}

void UPopulationContainer::ChangeMaxSize(int32 Change)
{
	if (Change > 0)
	{
		IncreaseMaxSize(Change);
	}
	else if (Change < 0)
	{
		DecreaseMaxSize(-Change);
	}
}

void UPopulationContainer::IncreaseMaxSize(int32 Change)
{
	if (Change <= 0) return;
	FPopulation OldPop = Population;
	Population.MaxSize += Change;
	PopulationChanged(Population - OldPop);
}

void UPopulationContainer::DecreaseMaxSize(int32 Change)
{
	if (Change <= 0) return;
	FPopulation OldPop = Population;
	int32 Effective_Change = Change > Population.MaxSize ? Population.MaxSize : Change;
	Population.MaxSize -= Effective_Change;
	// In case that max size is smaller than size, DecreaseSize will call the delegate
	if (Population.MaxSize < Population.Size)
	{
		DecreaseSize(Population.Size - Population.MaxSize);
	}
	else
	{
		PopulationChanged(Population - OldPop);
	}
}

void UPopulationContainer::ChangeFollower(ECultureLoyalty Culture, int32 Change)
{
	if (Change > 0)
	{
		IncreaseFollower(Culture, Change);
	}
	else if (Change < 0)
	{
		DecreaseFollower(Culture, -Change);
	}
}

void UPopulationContainer::IncreaseFollower(ECultureLoyalty Culture, int32 Change)
{
	if (Change <= 0) return;
	FPopulation OldPop = Population;
	int32 OldValue = GetFollower(Culture);
	int32 NewValue = OldValue + Change;
	if (NewValue > Population.Size) NewValue = Population.Size;
	int32 Effective_Change = NewValue - OldValue;
	if (Effective_Change == 0) return;
	Population.SetFollower(Culture, NewValue);

	// If everyone follows Target Culture
	if (NewValue == Population.Size)
	{
		// all other Cultures have 0 follower now
		if (Culture != ECultureLoyalty::Colonists) Population.FollowerColonists = 0;
		if (Culture != ECultureLoyalty::Guardian1) Population.FollowerGuardian1 = 0;
		if (Culture != ECultureLoyalty::Guardian2) Population.FollowerGuardian2 = 0;
		if (Culture != ECultureLoyalty::Guardian3) Population.FollowerGuardian3 = 0;
		if (Culture != ECultureLoyalty::Guardian4) Population.FollowerGuardian4 = 0;
	}
	for (int32 i = 0; i < Effective_Change; i++)
	{
		ChangeFollowerWeightedRandomBy(-1, Culture);
	}
	PopulationChanged(Population - OldPop);
}

void UPopulationContainer::DecreaseFollower(ECultureLoyalty Culture, int32 Change)
{
	if (Change <= 0) return;
	FPopulation OldPop = Population;
	int32 OldValue = GetFollower(Culture);
	int32 NewValue = OldValue - Change;

	// Target Religion has no followers now
	if (NewValue < 0) NewValue = 0;

	int32 Effective_Change = NewValue - OldValue;
	if (Effective_Change == 0) return;
	Population.SetFollower(Culture, NewValue);

	// if Guardian gets reduced, Colonist gets increased
	if (Culture != ECultureLoyalty::Colonists)
	{
		Population.FollowerColonists -= Effective_Change;
	}
	else
	{
		// If Nobody follows Guardians, Weighted Random doesn't work, so we use Full Random
		if (Population.GetNativeFollowers() == 0)
		{
			for (int32 i = Effective_Change; i < 0; ++i)
			{
				AddOneFollowerToGuardiansFullRandom();
			}
		}
		else // Otherwise Weighted Random works
		{
			for (int32 i = Effective_Change; i < 0; ++i)
			{
				ChangeFollowerWeightedRandomBy(1, Culture);
			}
		}
	}
	PopulationChanged(Population - OldPop);
}

void UPopulationContainer::ChangeMood(EMood Mood, int32 Change)
{
	if (Change > 0)
	{
		IncreaseMood(Mood, Change);
	}
	else if (Change < 0)
	{
		DecreaseMood(Mood, -Change);
	}
}

void UPopulationContainer::IncreaseMood(EMood Mood, int32 Change)
{
	if (Change <= 0) return;
	FPopulation OldPop = Population;
	int32 OldValue = GetMood(Mood);
	int32 NewValue = OldValue + Change;
	if (NewValue > Population.Size) NewValue = Population.Size;
	int32 Effective_Change = NewValue - OldValue;
	if (Effective_Change == 0) return;
	Population.SetMood(Mood, NewValue);
	// If Aggressive gets Increased, first decrease Neutral, then Fearful pops
	if (Mood == EMood::Angry)
	{
		if (Population.MoodContent >= Effective_Change)
		{
			Population.MoodContent -= Effective_Change;
		}
		else
		{
			Population.MoodFear -= Effective_Change - Population.MoodContent;
			Population.MoodContent = 0;
		}
	}
	// If Fearful gets Increased, first decrease Neutral, then Aggressive pops
	else if (Mood == EMood::Fear)
	{
		if (Population.MoodContent >= Effective_Change)
		{
			Population.MoodContent -= Effective_Change;
		}
		else
		{
			Population.MoodAngry -= Effective_Change - Population.MoodContent;
			Population.MoodContent = 0;
		}
	}
	// If Neutral gets increased, use Random to decrease Fearful and Aggressive pops
	else if (Mood == EMood::Content)
	{
		int32 FearReduce = FMath::RandRange(0, Effective_Change);
		if (FearReduce > Population.MoodFear)
		{
			FearReduce = Population.MoodFear;
		}
		Population.MoodFear -= FearReduce;
		Population.MoodAngry -= Effective_Change - FearReduce;
	}
	PopulationChanged(Population - OldPop);
}

void UPopulationContainer::DecreaseMood(EMood Mood, int32 Change)
{
	if (Change <= 0) return;
	FPopulation OldPop = Population;
	int32 OldValue = GetMood(Mood);
	int32 NewValue = OldValue - Change;
	if (NewValue < 0) NewValue = 0;
	int32 Effective_Change = NewValue - OldValue;
	if (Effective_Change == 0) return;
	Population.SetMood(Mood, NewValue);
	// If Neutral gets reduced, use Random to increase Fearful and Aggressive pops
	if (Mood == EMood::Content)
	{
		int32 Random = FMath::RandRange(0, Effective_Change);
		Population.MoodAngry += Effective_Change - Random;
		Population.MoodFear += Random;
	}
	else
	{
		Population.MoodContent += Effective_Change;
	}
	PopulationChanged(Population - OldPop);
}

FPopulation UPopulationContainer::ExtractRandomPopForArmy(int32 Amount, USettlementBalance* Balance)
{
	FPopulation OldPop = Population;
	if (Amount > Population.Size) Amount = Population.Size;
	for (int32 i = 0; i < Amount; ++i)
	{
		ChangeFollowerWeightedRandomBy(-1);
		int32 ContentBias = Population.MoodContent * Balance->ContentMoodPickBias;
		int32 AngryBias = Population.MoodAngry * Balance->AngryMoodPickBias;
		int32 FearBias = Population.MoodFear * Balance->FearMoodPickBias;
		int32 TotalMood = ContentBias + AngryBias + FearBias;
		// Select a random dude with mood
		int32 Cursor = FMath::RandRange(0, TotalMood);
		// Find Selected mood
		if (Cursor < ContentBias)
		{
			--Population.MoodContent;
		}
		else if (Cursor < ContentBias + AngryBias)
		{
			--Population.MoodAngry;
		}
		else
		{
			--Population.MoodFear;
		}
	}
	Population.Size -= Amount;
	return OldPop - Population;
}

void UPopulationContainer::ChangeFollowerWeightedRandomBy(int32 Change, ECultureLoyalty Exclude)
{
	TArray<ECultureLoyalty> Cultures;
	TArray<int32> Values;
	int32 TotalFollower = 0;
	if (Population.FollowerColonists > 0 && ECultureLoyalty::Colonists != Exclude)
	{
		Cultures.Add(ECultureLoyalty::Colonists);
		Values.Add(Population.FollowerColonists);
		TotalFollower += Population.FollowerColonists;
	}
	if (Population.FollowerGuardian1 > 0 && ECultureLoyalty::Guardian1 != Exclude)
	{
		Cultures.Add(ECultureLoyalty::Guardian1);
		Values.Add(Population.FollowerGuardian1);
		TotalFollower += Population.FollowerGuardian1;
	}
	if (Population.FollowerGuardian2 > 0 && ECultureLoyalty::Guardian2 != Exclude)
	{
		Cultures.Add(ECultureLoyalty::Guardian2);
		Values.Add(Population.FollowerGuardian2);
		TotalFollower += Population.FollowerGuardian2;
	}
	if (Population.FollowerGuardian3 > 0 && ECultureLoyalty::Guardian3 != Exclude)
	{
		Cultures.Add(ECultureLoyalty::Guardian3);
		Values.Add(Population.FollowerGuardian3);
		TotalFollower += Population.FollowerGuardian3;
	}
	if (Population.FollowerGuardian4 > 0 && ECultureLoyalty::Guardian4 != Exclude)
	{
		Cultures.Add(ECultureLoyalty::Guardian4);
		Values.Add(Population.FollowerGuardian4);
		TotalFollower += Population.FollowerGuardian4;
	}
	// No Weights so this doesn't make sense, use default culture in this case
	if (TotalFollower == 0)
	{
		if (Change > 0)
			Population.SetFollower(DefaultCulture, Change);
		return;
	}
	// Select a random Believer
	int32 cursor = FMath::RandRange(0, TotalFollower - 1);
	// Find Selected Religion
	for (int32 i = 0; i < Values.Num(); i++)
	{
		cursor -= Values[i];
		if (cursor < 0)
		{
			// This Religion is selected
			Population.SetFollower(Cultures[i], Values[i] + Change);
			return;
		}
	}
}

void UPopulationContainer::AddOneFollowerToGuardiansFullRandom()
{
	switch (FMath::RandRange(0, 3))
	{
	case 0:
		++Population.FollowerGuardian1;
	case 1:
		++Population.FollowerGuardian2;
	case 2:
		++Population.FollowerGuardian3;
	case 3:
		++Population.FollowerGuardian4;
	default: ;
	}
}

void UPopulationContainer::SubtractOneMoodWeightedRandom()
{
	int32 TotalMood = Population.MoodContent + Population.MoodAngry + Population.MoodFear;
	// Select a random dude with mood
	int32 Cursor = FMath::RandRange(0, TotalMood - 1);
	// Find Selected mood
	if (Cursor < Population.MoodContent)
	{
		--Population.MoodContent;
	}
	else if (Cursor < Population.MoodContent + Population.MoodAngry)
	{
		--Population.MoodAngry;
	}
	else
	{
		--Population.MoodFear;
	}
}

void UPopulationContainer::PopulationChanged(const FPopulation& Change)
{
	RecalculateCombatValues();
	OnPopulationChanged.Broadcast(Change);
}

// ---------------------------------------------------------
// Getters and Setters

int32 UPopulationContainer::GetSize()
{
	return Population.Size;
}

FPopulation UPopulationContainer::GetPopulation()
{
	return Population;
}


int32 UPopulationContainer::GetFollower(ECultureLoyalty Culture) const
{
	return Population.GetFollower(Culture);
}

int32 UPopulationContainer::GetNativeFollowers()
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
	return Population.GetMood(Mood);
}

void UPopulationContainer::GetAllMood(int32& Content, int32& Angry, int32& Fear)
{
	Content = Population.MoodContent;
	Fear = Population.MoodFear;
	Angry = Population.MoodAngry;
}
