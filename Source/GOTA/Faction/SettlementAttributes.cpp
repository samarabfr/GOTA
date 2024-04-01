// Fill out your copyright notice in the Description page of Project Settings.


#include "SettlementAttributes.h"
#include "Net/UnrealNetwork.h"

void USettlementAttributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USettlementAttributes, Population);
	DOREPLIFETIME(USettlementAttributes, MaxPopulation);
}

//====================================================================
//--------------------Population
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void USettlementAttributes::AddPopulation(const float Addend, float& Effective_Change)
{
	float Before = Population;
	SetPopulation(Population + Addend);
	Effective_Change = Population - Before;
}

void USettlementAttributes::MultiplyPopulation(const float Factor, float& Effective_Change)
{
	float Before = Population;
	SetPopulation(Population * Factor);
	Effective_Change = Population - Before;
}

void USettlementAttributes::OnRep_Population(float NewPopulation)
{
	OnPopulationChanged.Broadcast(NewPopulation);
	OnAnyAttributeChanged.Broadcast();
}

void USettlementAttributes::SetPopulation(float NewPopulation)
{
	Population = FMath::Min(FMath::Max(NewPopulation, 0.0f), MaxPopulation);
	OnPopulationChanged.Broadcast(Population);
	OnAnyAttributeChanged.Broadcast();
}

float USettlementAttributes::GetPopulation()
{
	return Population;
}

//====================================================================
//--------------------MaxPopulation
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void USettlementAttributes::AddMaxPopulation(const float Addend, float& Effective_Change)
{
	float Before = MaxPopulation;
	SetMaxPopulation(MaxPopulation + Addend);
	Effective_Change = MaxPopulation - Before;
}

void USettlementAttributes::MultiplyMaxPopulation(const float Factor, float& Effective_Change)
{
	float Before = MaxPopulation;
	SetMaxPopulation(MaxPopulation * Factor);
	Effective_Change = MaxPopulation - Before;
}

void USettlementAttributes::OnRep_MaxPopulation(float NewMaxPopulation)
{
	OnMaxPopulationChanged.Broadcast(NewMaxPopulation);
	OnAnyAttributeChanged.Broadcast();
}

void USettlementAttributes::SetMaxPopulation(float NewMaxPopulation)
{
	MaxPopulation = FMath::Max(NewMaxPopulation, 0.0f);
	OnMaxPopulationChanged.Broadcast(MaxPopulation);
	OnAnyAttributeChanged.Broadcast();
}

float USettlementAttributes::GetMaxPopulation()
{
	return MaxPopulation;
}

//====================================================================
//--------------------Food
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void USettlementAttributes::AddFood(const float Addend, float& Effective_Change)
{
	float Before = Food;
	SetFood(Food + Addend);
	Effective_Change = Food - Before;
}

void USettlementAttributes::MultiplyFood(const float Factor, float& Effective_Change)
{
	float Before = Food;
	SetFood(Food * Factor);
	Effective_Change = Food - Before;
}

void USettlementAttributes::OnRep_Food(float NewFood)
{
	OnFoodChanged.Broadcast(NewFood);
	OnAnyAttributeChanged.Broadcast();
}

void USettlementAttributes::SetFood(float NewFood)
{
	Food = FMath::Max(NewFood, 0.0f);
	OnFoodChanged.Broadcast(Food);
	OnAnyAttributeChanged.Broadcast();
}

float USettlementAttributes::GetFood()
{
	return Food;
}

//====================================================================
//--------------------ColonistReligion
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	
void USettlementAttributes::AddColonistReligion(const float Addend, float& Effective_Change)
{
	float Before = ColonistReligion;
	SetColonistReligion(ColonistReligion + Addend);
	Effective_Change = ColonistReligion - Before;
}

void USettlementAttributes::MultiplyColonistReligion(const float Factor, float& Effective_Change)
{
	float Before = ColonistReligion;
	SetColonistReligion(ColonistReligion * Factor);
	Effective_Change = ColonistReligion - Before;
}

void USettlementAttributes::OnRep_ColonistReligion(float NewColonistReligion)
{
	OnColonistReligionChanged.Broadcast(NewColonistReligion);
	OnAnyAttributeChanged.Broadcast();
}

void USettlementAttributes::SetColonistReligion(float NewColonistReligion)
{
	ColonistReligion = FMath::Min(FMath::Max(NewColonistReligion, 0.0f), Population);
	NativeReligion = Population - ColonistReligion;
	OnColonistReligionChanged.Broadcast(ColonistReligion);
	OnNativeReligionChanged.Broadcast(NativeReligion);
	OnAnyAttributeChanged.Broadcast();
}

float USettlementAttributes::GetColonistReligion()
{
	return ColonistReligion;
}

//====================================================================
//--------------------NativeReligion
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	
void USettlementAttributes::AddNativeReligion(const float Addend, float& Effective_Change)
{
	float Before = NativeReligion;
	SetNativeReligion(NativeReligion + Addend);
	Effective_Change = NativeReligion - Before;
}

void USettlementAttributes::MultiplyNativeReligion(const float Factor, float& Effective_Change)
{
	float Before = NativeReligion;
	SetNativeReligion(NativeReligion * Factor);
	Effective_Change = NativeReligion - Before;
}

void USettlementAttributes::OnRep_NativeReligion(float NewNativeReligion)
{
	OnNativeReligionChanged.Broadcast(NewNativeReligion);
	OnAnyAttributeChanged.Broadcast();
}

void USettlementAttributes::SetNativeReligion(float NewNativeReligion)
{
	NativeReligion = FMath::Min(FMath::Max(NewNativeReligion, 0.0f), Population);
	ColonistReligion = Population - NativeReligion;
	OnNativeReligionChanged.Broadcast(NativeReligion);
	OnColonistReligionChanged.Broadcast(ColonistReligion);
	OnAnyAttributeChanged.Broadcast();
}

float USettlementAttributes::GetNativeReligion()
{
	return NativeReligion;
}