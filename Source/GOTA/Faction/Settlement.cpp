// Fill out your copyright notice in the Description page of Project Settings.


#include "Settlement.h"
#include "Net/UnrealNetwork.h"

void ASettlement::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASettlement, ClaimColor);
	DOREPLIFETIME(ASettlement, Population);
	DOREPLIFETIME(ASettlement, Food);
	DOREPLIFETIME(ASettlement, Wood);
	DOREPLIFETIME(ASettlement, ColonistReligion);
	DOREPLIFETIME(ASettlement, NativeReligion);
}

ASettlement::ASettlement()
{
	Food = CreateDefaultSubobject<UGOTAAttribute>(TEXT("FoodAttribute"));
	Wood = CreateDefaultSubobject<UGOTAAttribute>(TEXT("WoodAttribute"));
	Population = CreateDefaultSubobject<UGOTAAttributeLimited>(TEXT("PopulationAttribute"));
}

//====================================================================
//--------------------ColonistReligion
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	
void ASettlement::AddColonistReligion(const float Addend, float& Effective_Change)
{
	float Before = ColonistReligion;
	SetColonistReligion(ColonistReligion + Addend);
	Effective_Change = ColonistReligion - Before;
}

void ASettlement::SubtractColonistReligion(const float Subtrahend, float& Effective_Change)
{
	float Before = ColonistReligion;
	SetColonistReligion(ColonistReligion - Subtrahend);
	Effective_Change = ColonistReligion - Before;
}

void ASettlement::MultiplyColonistReligion(const float Factor, float& Effective_Change)
{
	float Before = ColonistReligion;
	SetColonistReligion(ColonistReligion * Factor);
	Effective_Change = ColonistReligion - Before;
}

void ASettlement::OnRep_ColonistReligion(float NewColonistReligion)
{
	OnColonistReligionChanged.Broadcast(NewColonistReligion);
	OnAnyAttributeChanged.Broadcast();
}

void ASettlement::SetColonistReligion(float NewColonistReligion)
{
	ColonistReligion = FMath::Min(FMath::Max(NewColonistReligion, 0.0f), Population->Current);
	NativeReligion = Population->Current - ColonistReligion;
	OnColonistReligionChanged.Broadcast(ColonistReligion);
	OnNativeReligionChanged.Broadcast(NativeReligion);
	OnAnyAttributeChanged.Broadcast();
}

float ASettlement::GetColonistReligion()
{
	return ColonistReligion;
}

//====================================================================
//--------------------NativeReligion
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	
void ASettlement::AddNativeReligion(const float Addend, float& Effective_Change)
{
	float Before = NativeReligion;
	SetNativeReligion(NativeReligion + Addend);
	Effective_Change = NativeReligion - Before;
}

void ASettlement::SubtractNativeReligion(const float Subtrahend, float& Effective_Change)
{
	float Before = NativeReligion;
	SetNativeReligion(NativeReligion - Subtrahend);
	Effective_Change = NativeReligion - Before;
}

void ASettlement::MultiplyNativeReligion(const float Factor, float& Effective_Change)
{
	float Before = NativeReligion;
	SetNativeReligion(NativeReligion * Factor);
	Effective_Change = NativeReligion - Before;
}

void ASettlement::OnRep_NativeReligion(float NewNativeReligion)
{
	OnNativeReligionChanged.Broadcast(NewNativeReligion);
	OnAnyAttributeChanged.Broadcast();
}

void ASettlement::SetNativeReligion(float NewNativeReligion)
{
	NativeReligion = FMath::Min(FMath::Max(NewNativeReligion, 0.0f), Population->GetCurrent());
	ColonistReligion = Population->Current - NativeReligion;
	OnNativeReligionChanged.Broadcast(NativeReligion);
	OnColonistReligionChanged.Broadcast(ColonistReligion);
	OnAnyAttributeChanged.Broadcast();
}

float ASettlement::GetNativeReligion()
{
	return NativeReligion;
}