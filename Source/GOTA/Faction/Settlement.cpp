// Fill out your copyright notice in the Description page of Project Settings.


#include "Settlement.h"
#include "Net/UnrealNetwork.h"

void ASettlement::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASettlement, ExpectedFoodIncome);
	DOREPLIFETIME(ASettlement, Population);
	DOREPLIFETIME(ASettlement, MaxPopulation);
	DOREPLIFETIME(ASettlement, Food);
	DOREPLIFETIME(ASettlement, ColonistReligion);
	DOREPLIFETIME(ASettlement, NativeReligion);
	DOREPLIFETIME(ASettlement, ClaimColor);
}

ASettlement::ASettlement()
{
	Food = CreateDefaultSubobject<UGOTAAttribute>(TEXT("FoodAttribute"));
}


//====================================================================
//--------------------ExpectedFoodIncome
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void ASettlement::AddExpectedFoodIncome(const float Addend, float& Effective_Change)
{
	float Before = ExpectedFoodIncome;
	SetExpectedFoodIncome(ExpectedFoodIncome + Addend);
	Effective_Change = ExpectedFoodIncome - Before;
}

void ASettlement::SubtractExpectedFoodIncome(const float Subtrahend, float& Effective_Change)
{
	float Before = ExpectedFoodIncome;
	SetExpectedFoodIncome(ExpectedFoodIncome - Subtrahend);
	Effective_Change = ExpectedFoodIncome - Before;
}

void ASettlement::OnRep_ExpectedFoodIncome(float NewExpectedFoodIncome)
{
	OnExpectedFoodIncomeChanged.Broadcast(NewExpectedFoodIncome);
	OnAnyAttributeChanged.Broadcast();
}

void ASettlement::SetExpectedFoodIncome(float NewExpectedFoodIncome)
{
	ExpectedFoodIncome = NewExpectedFoodIncome;
	OnExpectedFoodIncomeChanged.Broadcast(ExpectedFoodIncome);
	OnAnyAttributeChanged.Broadcast();
}

float ASettlement::GetExpectedFoodIncome()
{
	return ExpectedFoodIncome - Population;
}

//====================================================================
//--------------------Population
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void ASettlement::AddPopulation(const float Addend, float& Effective_Change)
{
	float Before = Population;
	SetPopulation(Population + Addend);
	Effective_Change = Population - Before;
}

void ASettlement::SubtractPopulation(const float Subtrahend, float& Effective_Change)
{
	float Before = Population;
	SetPopulation(Population - Subtrahend);
	Effective_Change = Population - Before;
}

void ASettlement::MultiplyPopulation(const float Factor, float& Effective_Change)
{
	float Before = Population;
	SetPopulation(Population * Factor);
	Effective_Change = Population - Before;
}

void ASettlement::OnRep_Population(float NewPopulation)
{
	OnPopulationChanged.Broadcast(NewPopulation);
	OnAnyAttributeChanged.Broadcast();
	OnExpectedFoodIncomeChanged.Broadcast(ExpectedFoodIncome);
}

void ASettlement::SetPopulation(float NewPopulation)
{
	Population = FMath::Min(FMath::Max(NewPopulation, 0.0f), MaxPopulation);
	OnPopulationChanged.Broadcast(Population);
	OnAnyAttributeChanged.Broadcast();
	OnExpectedFoodIncomeChanged.Broadcast(ExpectedFoodIncome);
}

float ASettlement::GetPopulation()
{
	return Population;
}

//====================================================================
//--------------------MaxPopulation
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void ASettlement::AddMaxPopulation(const float Addend, float& Effective_Change)
{
	float Before = MaxPopulation;
	SetMaxPopulation(MaxPopulation + Addend);
	Effective_Change = MaxPopulation - Before;
}

void ASettlement::SubtractMaxPopulation(const float Subtrahend, float& Effective_Change)
{
	float Before = MaxPopulation;
	SetMaxPopulation(MaxPopulation - Subtrahend);
	Effective_Change = MaxPopulation - Before;
}

void ASettlement::MultiplyMaxPopulation(const float Factor, float& Effective_Change)
{
	float Before = MaxPopulation;
	SetMaxPopulation(MaxPopulation * Factor);
	Effective_Change = MaxPopulation - Before;
}

void ASettlement::OnRep_MaxPopulation(float NewMaxPopulation)
{
	OnMaxPopulationChanged.Broadcast(NewMaxPopulation);
	OnAnyAttributeChanged.Broadcast();
}

void ASettlement::SetMaxPopulation(float NewMaxPopulation)
{
	MaxPopulation = FMath::Max(NewMaxPopulation, 0.0f);
	OnMaxPopulationChanged.Broadcast(MaxPopulation);
	OnAnyAttributeChanged.Broadcast();
}

float ASettlement::GetMaxPopulation()
{
	return MaxPopulation;
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
	ColonistReligion = FMath::Min(FMath::Max(NewColonistReligion, 0.0f), Population);
	NativeReligion = Population - ColonistReligion;
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
	NativeReligion = FMath::Min(FMath::Max(NewNativeReligion, 0.0f), Population);
	ColonistReligion = Population - NativeReligion;
	OnNativeReligionChanged.Broadcast(NativeReligion);
	OnColonistReligionChanged.Broadcast(ColonistReligion);
	OnAnyAttributeChanged.Broadcast();
}

float ASettlement::GetNativeReligion()
{
	return NativeReligion;
}