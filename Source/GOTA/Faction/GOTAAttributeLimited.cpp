// Fill out your copyright notice in the Description page of Project Settings.


#include "GOTAAttributeLimited.h"
#include "Net/UnrealNetwork.h"

void UGOTAAttributeLimited::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGOTAAttributeLimited, Maximum);
}

void UGOTAAttributeLimited::SetCurrent(float NewValue)
{
	Current = FMath::Max(0, FMath::Min(NewValue, Maximum));
	OnChanged.Broadcast(0);
}

float UGOTAAttributeLimited::GetMaximum() const
{
	return Maximum;
}

void UGOTAAttributeLimited::SetMaximum(float NewValue)
{
	Maximum = FMath::Max(0, NewValue);
	OnChanged.Broadcast(0);
}

void UGOTAAttributeLimited::OnRep_Maximum()
{
	OnChanged.Broadcast(0);
}

void UGOTAAttributeLimited::AddMaximum(const float Addend, float& Effective_Change)
{
	float const Before = Maximum;
	SetMaximum(Maximum + Addend);
	Effective_Change = Maximum - Before;
}

void UGOTAAttributeLimited::SubtractMaximum(const float Subtrahend, float& Effective_Change)
{
	float const Before = Maximum;
	SetMaximum(Maximum - Subtrahend);
	Effective_Change = Maximum - Before;
}

void UGOTAAttributeLimited::MultiplyMaximum(const float Factor, float& Effective_Change)
{
	float const Before = Maximum;
	SetMaximum(Maximum * Factor);
	Effective_Change = Maximum - Before;
}