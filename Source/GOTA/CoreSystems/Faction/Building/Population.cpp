// Fill out your copyright notice in the Description page of Project Settings.


#include "Population.h"

#include "PopulationSettings.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// ------------------- Replication Setup -------------------

void UPopulation::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(UPopulation, Settings, Params)

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(UPopulation, Size, Params)
	DOREPLIFETIME_WITH_PARAMS(UPopulation, MaxSize, Params)
	DOREPLIFETIME_WITH_PARAMS(UPopulation, Angry, Params)
	DOREPLIFETIME_WITH_PARAMS(UPopulation, Fear, Params)
	DOREPLIFETIME_WITH_PARAMS(UPopulation, GrowthProgress, Params)
}

bool UPopulation::IsSupportedForNetworking() const
{
	return true;
}

// ------------------- LifeCycle -------------------

void UPopulation::S_Init(UPopulationSettings* InSettings)
{
	Settings = InSettings;
}

void UPopulation::S_Tick(const float DeltaSeconds)
{
	ApplyGrowth(DeltaSeconds);

	if (GrowthProgress >= 1)
	{
		const int32 Count = GrowthProgress;
		S_IncreaseSize(Count);
		GrowthProgress -= Count;
		MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, GrowthProgress, this)
	}
}

void UPopulation::C_Tick(const float DeltaSeconds)
{
	ApplyGrowth(DeltaSeconds);
}

// ------------------- Size -------------------

void UPopulation::OnRep_Size(const int16 OldValue)
{
	OnSizeChanged.Broadcast(Size - OldValue);
}

void UPopulation::S_ChangeSize(const int16 Change)
{
	if (Change > 0)
		S_IncreaseSize(Change);
	else if (Change < 0)
		S_DecreaseSize(-Change);
}

void UPopulation::S_IncreaseSize(const int16 Change)
{
	if (Change <= 0 || Size == MaxSize) return;
	const int16 OldSize = Size;
	Size = Size + Change > MaxSize ? MaxSize : Size + Change;
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Size, this)
	OnSizeChanged.Broadcast(Size - OldSize);
}

void UPopulation::S_DecreaseSize(const int16 Change)
{
	if (Change <= 0 || Size == 0) return;
	const int16 OldSize = Size;
	Size = Size - Change < 0 ? 0 : Size - Change;
	S_SubtractMoodWeightedRandom(OldSize - Size);
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Size, this)
	OnSizeChanged.Broadcast(Size - OldSize);
}

// ------------------- MaxSize -------------------

void UPopulation::OnRep_MaxSize(const int16 OldValue)
{
	OnMaxSizeChanged.Broadcast(MaxSize - OldValue);
}

void UPopulation::S_ChangeMaxSize(const int16 Change)
{
	if (Change > 0)
		S_IncreaseMaxSize(Change);
	else if (Change < 0)
		S_DecreaseMaxSize(-Change);
}

void UPopulation::S_IncreaseMaxSize(const int16 Change)
{
	if (Change <= 0) return;
	MaxSize += Change;
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, MaxSize, this)
	OnMaxSizeChanged.Broadcast(Change);
}

void UPopulation::S_DecreaseMaxSize(const int16 Change)
{
	if (Change <= 0 || MaxSize == 0) return;
	const int16 OldValue = MaxSize;
	MaxSize = MaxSize - Change < 0 ? 0 : MaxSize - Change;
	if (MaxSize < Size)
		S_DecreaseSize(Size - MaxSize);
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, MaxSize, this)
	OnMaxSizeChanged.Broadcast(MaxSize - OldValue);
}

// ------------------- Growth -------------------

void UPopulation::ApplyGrowth(const float DeltaSeconds)
{
	// If this Population is already filled, growth is ignored
	if (Size == MaxSize)
	{
		GrowthProgress = 0.0f;
		return;
	}

	// If Settlement is starving, this Population should not grow
	if (Settings->IsStarving)
		return;
	
	GrowthProgress += GetGrowth() * DeltaSeconds;
}

float UPopulation::GetGrowth() const
{
	return NeighborSize * Settings->GetGrowthPerNeighborPop() + GetSize() * Settings->GetGrowthPerOwnPop();
}

void UPopulation::NeighborChangedPopSize(int16 Amount)
{
	NeighborSize += Amount;
}

// ------------------- Mood -------------------

void UPopulation::S_SubtractMoodWeightedRandom(const int16 Change)
{
	const int16 OldFear = Fear;
	const int16 OldAngry = Angry;
	for (int i = 0; i < Change; ++i)
	{
		const int32 Cursor = FMath::RandRange(0, Size - 1);
		if (Cursor < Angry)
			--Angry;
		else if (Cursor < Angry + Fear)
			--Fear;
	}
	if (Angry != OldAngry)
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Angry, this)
		OnAngryChanged.Broadcast(Angry - OldAngry);
	}
	if (Fear != OldFear)
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Fear, this)
		OnFearChanged.Broadcast(Fear - OldFear);
	}
}

int16 UPopulation::GetContentMood() const
{
	return Size - Angry - Fear;
}

int16 UPopulation::GetMood(const EMood Mood) const
{
	switch (Mood)
	{
	case EMood::Content:
		return GetContentMood();
	case EMood::Angry:
		return Angry;
	case EMood::Fear:
		return Fear;
	default:
		return -1;
	}
}

void UPopulation::GetAllMood(int16& Content_, int16& Angry_, int16& Fear_) const
{
	Content_ = GetContentMood();
	Fear_ = Fear;
	Angry_ = Angry;
}

void UPopulation::S_ChangeMood(const EMood Mood, const int16 Change)
{
	if (Change > 0)
		S_IncreaseMood(Mood, Change);
	else if (Change < 0)
		S_DecreaseMood(Mood, -Change);
}

void UPopulation::S_IncreaseMood(const EMood Mood, const int16 Change)
{
	if (Mood == EMood::Angry)
		S_IncreaseAngry(Change);
	else if (Mood == EMood::Fear)
		IncreaseFear(Change);
}

void UPopulation::S_DecreaseMood(const EMood Mood, const int16 Change)
{
	if (Mood == EMood::Angry)
		S_DecreaseAngry(Change);
	else if (Mood == EMood::Fear)
		DecreaseFear(Change);
}

// ------------------- Angry Mood -------------------

void UPopulation::OnRep_Angry(const int16 OldValue)
{
	OnAngryChanged.Broadcast(Angry - OldValue);
}

void UPopulation::S_IncreaseAngry(const int16 Change)
{
	if (Change <= 0 || Angry == Size) return;
	const int16 OldAngry = Angry;
	Angry = Angry + Change > Size ? Size : Angry + Change;
	if (Angry + Fear > Size)
	{
		const int16 OldFear = Fear;
		Fear = Size - Angry;
		MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Fear, this)
		OnFearChanged.Broadcast(Fear - OldFear);
	}
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Angry, this)
	OnAngryChanged.Broadcast(Angry - OldAngry);
}

void UPopulation::S_DecreaseAngry(const int16 Change)
{
	if (Change <= 0 || Angry == 0) return;
	const int16 OldAngry = Angry;
	Angry = Angry - Change < 0 ? 0 : Angry - Change;
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Angry, this)
	OnAngryChanged.Broadcast(Angry - OldAngry);
}

// ------------------- Fear Mood -------------------

void UPopulation::OnRep_Fear(const int16 OldValue)
{
	OnFearChanged.Broadcast(Fear - OldValue);
}

void UPopulation::FearChanged(const int16 Change)
{
	OnFearChanged.Broadcast(Change);
}

void UPopulation::IncreaseFear(const int16 Change)
{
	if (Change <= 0 || Fear == Size) return;
	const int16 OldFear = Fear;
	Fear = Fear + Change > Size ? Size : Fear + Change;
	if (Fear + Angry > Size)
	{
		const int16 OldAngry = Angry;
		Angry = Size - Fear;
		MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Angry, this)
		OnAngryChanged.Broadcast(Angry - OldAngry);
	}
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Fear, this)
	OnFearChanged.Broadcast(Fear - OldFear);
}

void UPopulation::DecreaseFear(const int16 Change)
{
	if (Change <= 0 || Fear == 0) return;
	const int16 OldFear = Fear;
	Fear = Fear - Change < 0 ? 0 : Fear - Change;
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Fear, this)
	OnFearChanged.Broadcast(Fear - OldFear);
}
