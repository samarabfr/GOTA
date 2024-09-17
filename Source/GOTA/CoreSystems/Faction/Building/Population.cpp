// Fill out your copyright notice in the Description page of Project Settings.


#include "Population.h"
#include "GOTA/CoreSystems/Faction/Settlement/SettlementSettings.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

void UPopulation::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
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

UPopulation::UPopulation()
{
	ConstructorHelpers::FObjectFinder<USettlementSettings> DataAsset(
		TEXT("/Game/CoreSystems/Faction/DA_SettlementSettings"));
	SettlementSettings = DataAsset.Object;
}

void UPopulation::ServerTick(const float DeltaSeconds)
{
	ClientTick(DeltaSeconds);

	if (GrowthProgress >= SettlementSettings->PopulationGrowthThreshold)
	{
		int32 Count = GrowthProgress / SettlementSettings->PopulationGrowthThreshold;
		IncreaseSize(Count);
		GrowthProgress -= Count * SettlementSettings->PopulationGrowthThreshold;
		MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, GrowthProgress, this)
	}
}

void UPopulation::ClientTick(const float DeltaSeconds)
{
	if (Size == MaxSize)
		GrowthProgress = 0.0f;
	else
		GrowthProgress += Growth * DeltaSeconds;
}

// ---------------Changing Population Values-----------------------

void UPopulation::SetFaction(EFaction NewFaction)
{
	Faction = NewFaction;
}

void UPopulation::ChangeSize(const int16 Change)
{
	if (Change > 0)
		IncreaseSize(Change);
	else if (Change < 0)
		DecreaseSize(-Change);
}

void UPopulation::IncreaseSize(const int16 Change)
{
	if (Change <= 0 || Size == MaxSize) return;
	const int16 OldSize = Size;
	Size = Size + Change > MaxSize ? MaxSize : Size + Change;
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Size, this)
	SizeChanged(Size - OldSize);
}

void UPopulation::DecreaseSize(const int16 Change)
{
	if (Change <= 0 || Size == 0) return;
	const int16 OldSize = Size;
	Size = Size - Change < 0 ? 0 : Size - Change;
	SubtractMoodWeightedRandom(OldSize - Size);
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Size, this)
	SizeChanged(Size - OldSize);
}

void UPopulation::ChangeMaxSize(const int16 Change)
{
	if (Change > 0)
		IncreaseMaxSize(Change);
	else if (Change < 0)
		DecreaseMaxSize(-Change);
}

void UPopulation::IncreaseMaxSize(const int16 Change)
{
	if (Change <= 0) return;
	MaxSize += Change;
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, MaxSize, this)
	MaxSizeChanged(Change);
}

void UPopulation::DecreaseMaxSize(const int16 Change)
{
	if (Change <= 0 || MaxSize == 0) return;
	const int16 OldMaxSize = MaxSize;
	MaxSize = MaxSize - Change < 0 ? 0 : MaxSize - Change;
	if (MaxSize < Size)
		DecreaseSize(Size - MaxSize);
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, MaxSize, this)
	MaxSizeChanged(MaxSize - OldMaxSize);
}

void UPopulation::ChangeMood(const EMood Mood, const int16 Change)
{
	if (Change > 0)
		IncreaseMood(Mood, Change);
	else if (Change < 0)
		DecreaseMood(Mood, -Change);
}

void UPopulation::IncreaseMood(const EMood Mood, const int16 Change)
{
	if (Mood == EMood::Angry)
		IncreaseAngry(Change);
	else if (Mood == EMood::Fear)
		IncreaseFear(Change);
}

void UPopulation::DecreaseMood(const EMood Mood, const int16 Change)
{
	if (Mood == EMood::Angry)
		DecreaseAngry(Change);
	else if (Mood == EMood::Fear)
		DecreaseFear(Change);
}

void UPopulation::IncreaseAngry(const int16 Change)
{
	if (Change <= 0 || Angry == Size) return;
	const int16 OldAngry = Angry;
	Angry = Angry + Change > Size ? Size : Angry + Change;
	if (Angry + Fear > Size)
	{
		const int16 OldFear = Fear;
		Fear = Size - Angry;
		MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Fear, this)
		FearChanged(Fear - OldFear);
	}
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Angry, this)
	AngryChanged(Angry - OldAngry);
}

void UPopulation::DecreaseAngry(const int16 Change)
{
	if (Change <= 0 || Angry == 0) return;
	const int16 OldAngry = Angry;
	Angry = Angry - Change < 0 ? 0 : Angry - Change;
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Angry, this)
	AngryChanged(Angry - OldAngry);
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
		AngryChanged(Angry - OldAngry);
	}
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Fear, this)
	FearChanged(Fear - OldFear);
}

void UPopulation::DecreaseFear(const int16 Change)
{
	if (Change <= 0 || Fear == 0) return;
	const int16 OldFear = Fear;
	Fear = Fear - Change < 0 ? 0 : Fear - Change;
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Fear, this)
	FearChanged(Fear - OldFear);
}

void UPopulation::SubtractMoodWeightedRandom(const int16 Change)
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
		AngryChanged(Angry - OldAngry);
	}
	if (Fear != OldFear)
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Fear, this)
		FearChanged(Fear - OldFear);
	}
}

void UPopulation::OnRep_Size(const int16 OldValue)
{
	SizeChanged(Size - OldValue);
}

void UPopulation::OnRep_MaxSize(const int16 OldValue)
{
	MaxSizeChanged(Size - OldValue);
}

void UPopulation::OnRep_Angry(const int16 OldValue)
{
	AngryChanged(Size - OldValue);
}

void UPopulation::OnRep_Fear(const int16 OldValue)
{
	FearChanged(Size - OldValue);
}

void UPopulation::NeighborChangedPopSize(int16 Amount)
{
	Growth += Amount * SettlementSettings->PopGrowthPerNeighborPop;
}

void UPopulation::SizeChanged(const int16 Change)
{
	Growth += Change * SettlementSettings->PopGrowthPerOwnPop;
	OnSizeChanged.Broadcast(Change);
}

void UPopulation::MaxSizeChanged(const int16 Change)
{
	OnMaxSizeChanged.Broadcast(Change);
}

void UPopulation::AngryChanged(const int16 Change)
{
	OnAngryChanged.Broadcast(Change);
}

void UPopulation::FearChanged(const int16 Change)
{
	OnFearChanged.Broadcast(Change);
}

// ---------------------------------------------------------
// Getters and Setters

float UPopulation::GetGrowthThreshold() const
{
	return SettlementSettings->PopulationGrowthThreshold;
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
