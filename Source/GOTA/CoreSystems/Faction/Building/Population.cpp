// Fill out your copyright notice in the Description page of Project Settings.


#include "Population.h"
#include "GOTA/CoreSystems/Faction/Settlement/SettlementBalance.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

void UPopulation::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(UPopulation, Size, Params);
	DOREPLIFETIME_WITH_PARAMS(UPopulation, MaxSize, Params);
	DOREPLIFETIME_WITH_PARAMS(UPopulation, Angry, Params);
	DOREPLIFETIME_WITH_PARAMS(UPopulation, Fear, Params);
	DOREPLIFETIME_WITH_PARAMS(UPopulation, GrowthProgress, Params);
	DOREPLIFETIME_WITH_PARAMS(UPopulation, Growth, Params);
}

bool UPopulation::IsSupportedForNetworking() const
{
	return true;
}

UPopulation::UPopulation()
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
}

// ---------------Changing Population Values-----------------------

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
	Size = Size + Change > MaxSize ? MaxSize : Size + Change;
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Size, this)
	Changed();
}

void UPopulation::DecreaseSize(const int16 Change)
{
	if (Change <= 0 || Size == 0) return;
	const int16 OldSize = Size;
	Size = Size - Change < 0 ? 0 : Size - Change;
	for (int16 i = 0; i < OldSize - Size; ++i)
	{
		SubtractOneMoodWeightedRandom();
	}
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Size, this)
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Angry, this)
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Fear, this)
	Changed();
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
	Changed();
}

void UPopulation::DecreaseMaxSize(const int16 Change)
{
	if (Change <= 0 || MaxSize == 0) return;
	MaxSize = MaxSize - Change < 0 ? 0 : MaxSize - Change;
	if (MaxSize < Size)
		DecreaseSize(Size - MaxSize);
	else
		Changed();
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, MaxSize, this)
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
	Angry = Angry + Change > Size ? Size : Angry + Change;
	if (Angry + Fear > Size)
	{
		Fear = Size - Angry;
		MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Fear, this)
	}
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Angry, this)
	Changed();
}

void UPopulation::DecreaseAngry(const int16 Change)
{
	if (Change <= 0 || Angry == 0) return;
	Angry = Angry - Change < 0 ? 0 : Angry - Change;
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Angry, this)
	Changed();
}

void UPopulation::IncreaseFear(const int16 Change)
{
	if (Change <= 0 || Fear == Size) return;
	Fear = Fear + Change > Size ? Size : Fear + Change;
	if (Fear + Angry > Size)
	{
		Angry = Size - Fear;
		MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Angry, this)
	}
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Fear, this)
	Changed();
}

void UPopulation::DecreaseFear(const int16 Change)
{
	if (Change <= 0 || Fear == 0) return;
	Fear = Fear - Change < 0 ? 0 : Fear - Change;
	MARK_PROPERTY_DIRTY_FROM_NAME(UPopulation, Fear, this)
	Changed();
}

void UPopulation::SubtractOneMoodWeightedRandom()
{
	if (FMath::RandRange(0, Size - 1) > Fear)
		--Fear;
	else
		--Angry;
}

void UPopulation::Changed()
{
	OnChanged.Broadcast(this);
}

// ---------------------------------------------------------
// Getters and Setters

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

// --------------------Operators-----------------------

// Compound addition operator
UPopulation& UPopulation::operator+=(const UPopulation& Other)
{
	this->Size += Other.Size;
	this->MaxSize += Other.MaxSize;
	this->Angry += Other.Angry;
	this->Fear += Other.Fear;
	Changed();
	return *this; // Return a reference to *this for chaining
}

// Compound subtraction operator
UPopulation& UPopulation::operator-=(const UPopulation& Other)
{
	this->Size -= Other.Size;
	this->MaxSize -= Other.MaxSize;
	this->Angry -= Other.Angry;
	this->Fear -= Other.Fear;
	Changed();
	return *this; // Return a reference to *this for chaining
}
